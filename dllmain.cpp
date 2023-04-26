#pragma region Imports

// import windows
#include <vcruntime_string.h>
#include <corecrt_malloc.h>
#include <windows.h>
#include <Psapi.h>
#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include <random>
#include <sstream>
#include <cassert>

// import libraries
#include <MinHook.h>
#include "Memory/MCM.h"

//import SDK
#include "Memory/Utils/DrawUtils.h"
#include "Memory/Utils/Signatures.h"
#include "Memory/SDK/ClientInstance.h"

auto GetDllHMod(void) -> HMODULE {
    MEMORY_BASIC_INFORMATION info;
    size_t len = VirtualQueryEx(GetCurrentProcess(), (void*)GetDllHMod, &info, sizeof(info));
    assert(len == sizeof(info));
    return len ? (HMODULE)info.AllocationBase : NULL;
}

void DestoryClient() {
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    FreeLibraryAndExitThread(GetDllHMod(), 0);
}

//initializing sdk
class BitmapFont* font;
class ClientInstance* clientInst;
DrawUtils renderer = DrawUtils();
GuiData* acs;
Actor* localPlr;

typedef void(__thiscall* ClientInstanceHook)(ClientInstance* clientinstance, void*);
ClientInstanceHook _CIHook;

typedef void(__thiscall* LocalPlayer)(Actor* lp, void* a2);
LocalPlayer _player;

typedef void(__thiscall* RenderContextHook)(void*, MinecraftUIRenderContext* ctx);
RenderContextHook _RCHook;

typedef void(__thiscall* RenderTextHook)(void* a1, MinecraftUIRenderContext* ctx);
RenderTextHook _renderText;


void ClientInstanceCallback(ClientInstance* ci, void* a2) {
    clientInst = ci;
    acs = ci->getGuiData();
    font = ci->mcGame->defaultGameFont;
    _CIHook(ci, a2);
}

void playerCallback(Actor* lp, void* a2) {
    _player(lp, a2);
    if (lp->isLocalPlayer()) {
        localPlr = lp;
    }
}

int frames = 0;
void RenderContextCallback(void* a1, MinecraftUIRenderContext* ctx) {
    if (renderer.ctx == nullptr && font != nullptr)
        renderer.Init(ctx, acs, font);
    if (renderer.ctx == nullptr || font == nullptr) return;
    _RCHook(a1, ctx);

    frames++;
    if (frames == 3) {
        renderer.Draw(Vector2(0, 0), renderer.guiData->scaledResolution, UIColor(33, 33, 33, 150));
        //renderer.DrawString(Vector2(100, 100), UIColor(100, 100, 100), TextHolder("TestString"), font);

        frames = 0;
    }
}

void RenderTextCallback(void* a1, MinecraftUIRenderContext* ctx) {
    _renderText(a1, ctx);
}

void Init(LPVOID c) {
    if (MH_Initialize() == MH_OK) {
        uintptr_t clientInstance_Addr = MCM::findSig(Signatures::CLIENT_INSTANCE);
        if (MH_CreateHook((void*)clientInstance_Addr, &ClientInstanceCallback, reinterpret_cast<LPVOID*>(&_CIHook)) == MH_OK) {
            MH_EnableHook((void*)clientInstance_Addr);
        };

        uintptr_t localPlayer_Addr = MCM::findSig(Signatures::LOCAL_PLAYER);
        if (MH_CreateHook((void*)localPlayer_Addr, &playerCallback, reinterpret_cast<LPVOID*>(&_player)) == MH_OK) {
            MH_EnableHook((void*)localPlayer_Addr);
        };

        uintptr_t renderText_Addr = MCM::findSig(Signatures::RENDER_TEXT);
        if (MH_CreateHook((void*)renderText_Addr, &RenderTextCallback, reinterpret_cast<LPVOID*>(&_renderText)) == MH_OK) {
            MH_EnableHook((void*)renderText_Addr);
        };

        uintptr_t renderContext_Addr = MCM::findSig(Signatures::RENDER_CONTEXT);
        if (MH_CreateHook((void*)renderContext_Addr, &RenderContextCallback, reinterpret_cast<LPVOID*>(&_RCHook)) == MH_OK) {
            MH_EnableHook((void*)renderContext_Addr);
        };
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Init, hModule, 0, 0);
    }
    return TRUE;
}