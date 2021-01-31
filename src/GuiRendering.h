#pragma once
#include <SFML/System/Vector2.hpp>
#include <string>

struct GuiRenderInfo;
struct GuiRenderingState;

struct GuiRendering
{
	static void render(const GuiRenderInfo &guiRenderInfo);
	static void text(void *font, const char *text, float fontHeight, float x, float y);

	static void text(const char *text, float fontHeight, float x, float y);
	static void image(const void *image, float x, float y, float w, float h);
	static void imageShaded(const void *image, float x, float y, float w, float h, const void *shader);
    static void text(const std::string &text, float fontHeight, float x, float y);
    static void text(const char *text, float fontHeight, sf::Vector2f position);
    static void image(const void *image, sf::Vector2f position, float w, float h);
    static void line(float x0, float y0, float x1, float y1);
    static void line(sf::Vector2f pos0, sf::Vector2f pos1);
	static void circle(float x, float y, float r, bool filled = false);

	static void pushClipRect(float x, float y, float w, float h);
	static void pushAntiClipRect();
	static void popClipRect();

	static void startThread();
	static void endThread();

	static void flush(GuiRenderingState &result);
};
