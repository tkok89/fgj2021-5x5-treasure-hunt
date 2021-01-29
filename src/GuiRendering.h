#pragma once

struct GuiRenderInfo;
struct GuiRenderingState;

struct GuiRendering
{
	static void render(const GuiRenderInfo &guiRenderInfo);
	static void text(void *font, const char *text, float fontHeight, float x, float y);

	static void pushClipRect(float x, float y, float w, float h);
	static void pushAntiClipRect();
	static void popClipRect();

	static void startThread();
	static void endThread();

	static void flush(GuiRenderingState &result);
};
