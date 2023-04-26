#pragma once*
#include "../SDK/MinecraftUIRenderContext.h"
#include "../SDK/GuiData.h"

class DrawUtils
{
public:
	MinecraftUIRenderContext* ctx;
	GuiData* guiData;
	class BitmapFont* font;
	Vector2 pixelSize = Vector2(1, 1);

	void Init(MinecraftUIRenderContext* ctx, GuiData* guiData, class BitmapFont* font) {
		this->ctx = ctx;
		this->guiData = guiData;
		this->font = font;
	};

	auto MeasureText(std::string text, class BitmapFont* font, float scale) {
		TextHolder tekts = TextHolder(text);
		return ctx->getLineLength(font, &tekts, scale);
	};

	auto MeasureText(std::string text, class BitmapFont* font) {
		return this->MeasureText(text, font, 1.0f);
	};

	void Draw(Vector2 position, Vector2 size, UIColor colour) {
		if (ctx == nullptr) return;
		ctx->fillRectangle(Rect(position.x, position.x + size.x, position.y, position.y + size.y), colour, colour.a);
	};

	void Draw(Rect rectangle, UIColor colour) {
		if (ctx == nullptr) return;
		ctx->fillRectangle(Rect(rectangle.x, rectangle.z, rectangle.y, rectangle.w), colour, colour.a);
	};

	bool isMouseInRect(Rect pos, Vector2 mousePos)
	{
		if (pos.x < mousePos.x && pos.y < mousePos.y && pos.z > mousePos.x && pos.w > mousePos.y)
			return true;
		return false;
	}

	void DrawOutline(Vector2 position, Vector2 size, UIColor colour, float width) {
		if (ctx == nullptr) return;
		ctx->drawRectangle(Rect(position.x, position.x + size.x, position.y, position.y + size.y), colour, colour.a, (int)width);
	};

	void DrawOutline(Vector2 position, Vector2 size, UIColor colour) {
		if (ctx == nullptr) return;
		ctx->drawRectangle(Rect(position.x, position.x + size.x, position.y, position.y + size.y), colour, colour.a, (int)(colour.a / 255.0f));
	};

	void DrawString(Vector2 position, UIColor colour, TextHolder text, class BitmapFont* font) {
		DrawString(position, colour, text, font, 1.f);
		ctx->flushText(0);
	};

	/*auto World2Screen(Vector3 position, Vector2& out) { // fixed w2s
		auto level = *clientInst->getLevelRender();

		return clientInst->WorldToScreen(
			level.getOrigin(),
			position,
			out,
			clientInst->getFov(),
			guiData->scaledResolution
		);
	};*/

	void DrawString(Vector2 position, UIColor colour, TextHolder text, class BitmapFont* font, float measureCalc) {
		if (ctx == nullptr) return;
		auto calc = Rect(position.x, position.x + 1000, position.y, position.y + 1000);
		CaretMeasureData measureCalc2 = CaretMeasureData();
		ctx->drawText(font, &calc, &text, colour, colour.a, nullptr, &measureCalc, &measureCalc2);
		ctx->flushText(0);
	};

	void FillRectAndDrawRect(Rect pos, UIColor colour1, UIColor colour2, float opacity, float opacity2, float lineWidth) {
		ctx->fillRectangle(pos, colour1, opacity);
		ctx->drawRectangle(pos, colour2, opacity2, lineWidth);
	};

	void DrawPixel(Vector2 position, UIColor colour) {
		Draw(position, pixelSize, colour);
	};

	void DrawLine(Vector2 firstPoint, Vector2 secondPoint, UIColor colour) {
		float x = secondPoint.x - firstPoint.x;
		float y = secondPoint.y - firstPoint.y;

		float max = std::fmaxf(std::fabs(x), std::fabs(y));

		x /= max;
		y /= max;

		for (int i = 0; i < max; ++i) { // v
			Vector2 calcPixel = Vector2(firstPoint.x, firstPoint.y);
			DrawPixel(calcPixel, colour);

			firstPoint.x += x;
			firstPoint.y += y;
		}
	};

	void SetPixelBounds(Vector2 size) {
		pixelSize = size;
	};

	void DrawTriangleOutline(Vector2 position, Vector2 size, UIColor colour) {
		Vector2 secondPoint = Vector2();
		secondPoint.x = position.x + size.x;
		secondPoint.y = position.y + size.y;

		Vector2 middlePoint = Vector2();
		middlePoint.x = position.x + (size.x / 2);
		middlePoint.y = position.y + (size.y / 2);

		DrawLine(middlePoint, secondPoint, colour);
		DrawLine(middlePoint, Vector2(position.x, secondPoint.y), colour);
		DrawLine(secondPoint, Vector2(position.x, secondPoint.y), colour);
	};

	void DrawTriangle(Vector2 position, Vector2 size, UIColor colour) { // insanely ineffective laggy code
		Vector2 secondPoint = Vector2();
		secondPoint.x = position.x + size.x;
		secondPoint.y = position.y + size.y;

		float x = secondPoint.x - position.x;
		float y = secondPoint.y - position.y;

		float max = std::fmaxf(std::fabs(x), std::fabs(y));

		x /= max;
		y /= max;

		for (int i = 0; i < max / 2; ++i) { // v
			Vector2 calcPixel = Vector2(position.x, position.y);
			Vector2 calcSize = Vector2(size.x - (2 * i), size.y - (2 * i));
			DrawTriangleOutline(calcPixel, calcSize, colour);

			position.x += x;
			position.y += y;
		}
	};

	// from imgui
	static inline float ImFmod(float x, float y) { return fmodf(x, y); }
	static inline void hsvToRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b) {
		if (s == 0.0f) {
			out_r = out_g = out_b = v;
			return;
		}
		h = ImFmod(h, 1.0f) / (60.0f / 360.0f);
		int i = (int)h;
		float f = h - (float)i;
		float p = v * (1.0f - s);
		float q = v * (1.0f - s * f);
		float t = v * (1.0f - s * (1.0f - f));
		switch (i) {
		case 0:
			out_r = v;
			out_g = t;
			out_b = p;
			break;
		case 1:
			out_r = q;
			out_g = v;
			out_b = p;
			break;
		case 2:
			out_r = p;
			out_g = v;
			out_b = t;
			break;
		case 3:
			out_r = p;
			out_g = q;
			out_b = v;
			break;
		case 4:
			out_r = t;
			out_g = p;
			out_b = v;
			break;
		case 5:
		default:
			out_r = v;
			out_g = p;
			out_b = q;
			break;
		}
	}

	// Returns the current millisecond ( from stack overflow )
	__int64 getCurrentMs() {
		FILETIME f;
		GetSystemTimeAsFileTime(&f);
		(long long)f.dwHighDateTime;
		__int64 nano = ((__int64)f.dwHighDateTime << 32LL) + (__int64)f.dwLowDateTime;
		return (nano - 116444736000000000LL) / 10000;
	}

	// Gets HSV color by current MS
	UIColor getRainbow(float speedInSeconds, float s, float v, float red, float green, float blue, float trans)
	{
		float hue = ((getCurrentMs()) % (int)(((int)speedInSeconds) * 1000)) / (float)(((int)speedInSeconds) * 1000);
		float r, g, b = 0;
		hsvToRGB(hue, s, v, r, g, b);
		return UIColor(r * red, g * green, b * blue, trans);
	}

	UIColor getRainbow(float speedInSeconds, float s, float v, long index, float red, float green, float blue, float trans)
	{
		float hue = ((getCurrentMs() + index) % (int)(((int)speedInSeconds) * 1000)) / (float)(((int)speedInSeconds) * 1000);
		float r, g, b = 0;
		hsvToRGB(hue, s, v, r, g, b);
		return UIColor(r * red, g * green, b * blue, trans);
	}
};