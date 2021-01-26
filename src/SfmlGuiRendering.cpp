#include "SfmlGuiRendering.h"

#include "GuiRendering.h"
#include "GuiRenderingState.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <assert.h>

static sf::Vector2f g_resolution = sf::Vector2f(1280, 720);

void SfmlGuiRendering::setResolution(float x, float y)
{
	g_resolution = { x, y };
}

template<typename T>
sf::Color sfColor(T c)
{
	sf::Uint8 r = sf::Uint8(255 * c.r);
	sf::Uint8 g = sf::Uint8(255 * c.g);
	sf::Uint8 b = sf::Uint8(255 * c.b);
	sf::Uint8 a = sf::Uint8(255 * c.a);
	return sf::Color(r, g, b, a);
}

void SfmlGuiRendering::flush(sf::RenderTarget &renderTarget)
{
	GuiRenderingState guiRenderingState;

	GuiRendering::startThread();
	GuiRendering::flush(guiRenderingState);
	GuiRendering::endThread();

	SfmlGuiRendering::draw(renderTarget, guiRenderingState);
}

void SfmlGuiRendering::draw(sf::RenderTarget &renderTarget, const GuiRenderingState &guiRenderingState)
{
	for (const GuiRenderInfo &i : guiRenderingState.renderInfos)
	{
		sf::Vector2f pos(i.x * g_resolution.y + g_resolution.x * 0.5f, i.y * g_resolution.y + 0.5f * g_resolution.y);
		
		if (i.type == GuiRenderInfoType::Image)
		{
			sf::Vector2f size(i.w * g_resolution.y, i.h * g_resolution.y);
			const sf::Texture *texture = static_cast<const sf::Texture *>(i.image);
			assert(texture);

			sf::RenderStates states = sf::RenderStates::Default;
			states.texture = texture;

			sf::Vertex vertices[4] = {};
			{
				float left = pos.x;
				float right = pos.x + size.x;
				float top = pos.y;
				float bottom = pos.y + size.y;
				vertices[0].position = sf::Vector2f(left, top);
				vertices[1].position = sf::Vector2f(left, bottom);
				vertices[2].position = sf::Vector2f(right, top);
				vertices[3].position = sf::Vector2f(right, bottom);
			}
			
			{
				sf::Color color = sfColor(i.color);
				vertices[0].color = color;
				vertices[1].color = color;
				vertices[2].color = color;
				vertices[3].color = color;
			}
			
			{
				sf::Vector2u textureSize = texture->getSize();
				float left = i.uvX * textureSize.x;
				float right = (i.uvX + i.uvW) * textureSize.x;
				float top = i.uvY * textureSize.y;
				float bottom = (i.uvY + i.uvH) * textureSize.y;
				vertices[0].texCoords = sf::Vector2f(left, top);
				vertices[1].texCoords = sf::Vector2f(left, bottom);
				vertices[2].texCoords = sf::Vector2f(right, top);
				vertices[3].texCoords = sf::Vector2f(right, bottom);
			}
			
			renderTarget.draw(vertices, 4, sf::TriangleStrip, states);
		}
		else if (i.type == GuiRenderInfoType::Text)
		{
			sf::Text *text = static_cast<sf::Text *>(i.font);
			text->setString(i.strPtr);
			text->setPosition(pos);
			text->setCharacterSize(unsigned(i.fontHeight * g_resolution.y));
			text->setFillColor(sfColor(i.color));
			text->setOutlineThickness(i.outline);
			text->setOutlineColor(sfColor(i.outlineColor));
			renderTarget.draw(*text);
		}
	}
}
