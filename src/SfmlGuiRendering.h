#pragma once

struct GuiRenderingState;
namespace sf { class RenderTarget; }
namespace sf { class Font; }

struct SfmlGuiRendering
{
	static void setResolution(float x, float y);
	static void setDefaultFont(sf::Font &font);

	static void flush(sf::RenderTarget &window);
	static void draw(sf::RenderTarget &window, const GuiRenderingState &guiRenderingState);
};
