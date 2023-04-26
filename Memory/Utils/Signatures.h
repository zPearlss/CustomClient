#pragma once
#include <string>

class Signatures {
public:
    static const char* CLIENT_INSTANCE;
    static const char* RENDER_CONTEXT;
    static const char* RENDER_TEXT;
    static const char* LOCAL_PLAYER;
};

const char* Signatures::CLIENT_INSTANCE = "48 8B 01 48 8D 54 24 ? FF 90 ? ? ? ? 90 48 8B 08 48 85 ? 0F 84 ? ? ? ? 48 8B 58 08 48 85 DB 74 0B F0 FF 43 08 48 8B 08 48 8B 58 08 48 89 4C 24 20 48 89 5C 24 28 48 8B 09 48 8B 01 4C 8B C7 48 8B";
const char* Signatures::RENDER_CONTEXT = "48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 B8 0F 29 78 A8 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA 48 89 54 24 ? 4C 8B";
const char* Signatures::RENDER_TEXT = "48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 18 FF FF FF 48 81 EC B0 01 00 00 0F 29 70 B8 0F 29 78 A8 44 0F 29 40 98 44 0F 29 48 88 44 0F 29 90 78 FF FF FF 44 0F 29 98 68 FF FF FF 44 0F 29 A0 58 FF FF FF 48 8B 05 9F 93 1A 03 48 33 C4 48 89 45";
const char* Signatures::LOCAL_PLAYER = "73 ? B0 ? C3 32 C0 C3 CC CC CC CC CC 48 83 EC";