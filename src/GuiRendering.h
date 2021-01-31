#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
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
    static void image(const void *image, sf::Vector2f position, sf::Vector2f size);
    static void image(const void *image, sf::Vector2f position, float w, float h);
    static void line(float x0, float y0, float x1, float y1);
    static void line(sf::Vector2f pos0, sf::Vector2f pos1);
	static void circle(float x, float y, float r, bool filled = false);
	static void triangle(sf::Vector2f pos0, sf::Vector2f pos1, sf::Vector2f pos2, sf::Color c);
	static void rect(sf::Vector2f pos, sf::Vector2f size, sf::Color c, bool filled);

	static bool button(const char *text, sf::Vector2f topLeft, sf::Vector2f size);


	static void pushClipRect(float x, float y, float w, float h);
	static void pushAntiClipRect();
	static void popClipRect();

	static void startThread();
	static void endThread();

	static void flush(GuiRenderingState &result);
};
