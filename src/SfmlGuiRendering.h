#pragma once

struct GuiRenderingState;
namespace sf { class RenderTarget; }

struct SfmlGuiRendering
{
	static void setResolution(float x, float y);

	static void flush(sf::RenderTarget &window);
	static void draw(sf::RenderTarget &window, const GuiRenderingState &guiRenderingState);
};
