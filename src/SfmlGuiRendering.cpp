#include "SfmlGuiRendering.h"

#include "GuiRendering.h"
#include "GuiRenderingState.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <cmath>

#include <assert.h>

static sf::Vector2f g_resolution = sf::Vector2f(1280, 720);
static sf::Text g_text;

void SfmlGuiRendering::setResolution(float x, float y)
{
	g_resolution = { x, y };
}

void SfmlGuiRendering::setDefaultFont(sf::Font &font)
{
	g_text.setFont(font);
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
			
			if (i.shader)
				states.shader = static_cast<const sf::Shader *>(i.shader);

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
			if (!text)
				text = &g_text;
			if (i.strPtr)
				text->setString(i.strPtr);
			text->setPosition(pos);
			text->setCharacterSize(unsigned(i.fontHeight * g_resolution.y));
			text->setFillColor(sfColor(i.color));
			text->setOutlineThickness(i.outline);
			text->setOutlineColor(sfColor(i.outlineColor));
			renderTarget.draw(*text);
		}
		else if (i.type == GuiRenderInfoType::Line)
		{
			sf::Vector2f pos2(i.w * g_resolution.y + g_resolution.x * 0.5f, i.h * g_resolution.y + 0.5f * g_resolution.y);
			sf::RenderStates states = sf::RenderStates::Default;

			sf::Vertex vertices[2] = {};
			{
				vertices[0].position = pos;
				vertices[1].position = pos2;
			}

			{
				sf::Color color = sfColor(i.color);
				vertices[0].color = color;
				vertices[1].color = color;
			}

			{
				vertices[0].texCoords = sf::Vector2f(0,0);
				vertices[1].texCoords = sf::Vector2f(1,1);
			}

			renderTarget.draw(vertices, 2, sf::Lines, states);
		}
		else if (i.type == GuiRenderInfoType::Circle)
		{
			float r = i.w * g_resolution.y;
			bool filled = i.h != 0;
			sf::Vector2f pos2(i.w * g_resolution.y + g_resolution.x * 0.5f, i.h * g_resolution.y + 0.5f * g_resolution.y);
			sf::RenderStates states = sf::RenderStates::Default;

			const int polyCount = 33;
			sf::Vertex vertices[polyCount] = {};
			const sf::Color color = sfColor(i.color);
			if (filled)
				vertices[0].position = pos;
			else
				vertices[0].position = pos + sf::Vector2f(0, r);

			vertices[0].color = color;
			vertices[0].texCoords = sf::Vector2f(0, 0);
			for (int i = 1; i < polyCount; ++i)
			{
				const float pi = 3.14159f;
				vertices[i].position.x = pos.x + r * sinf((i - 1.0f) / float(polyCount - 2.0f) * pi * 2.0f);
				vertices[i].position.y = pos.y + r * cosf((i - 1.0f) / float(polyCount - 2.0f) * pi * 2.0f);
				vertices[i].color = color;
				vertices[i].texCoords = sf::Vector2f(1, 1);
			}

			if (filled)
				renderTarget.draw(vertices, polyCount, sf::TriangleFan, states);
			else
				renderTarget.draw(vertices, polyCount, sf::LineStrip, states);
		}
		else if (i.type == GuiRenderInfoType::Triangle)
		{
			sf::Vector2f pos1(i.w * g_resolution.y + g_resolution.x * 0.5f, i.h * g_resolution.y + 0.5f * g_resolution.y);
			sf::Vector2f pos2(i.x2 *g_resolution.y + g_resolution.x * 0.5f, i.y2 *g_resolution.y + 0.5f * g_resolution.y);
			sf::RenderStates states = sf::RenderStates::Default;

			sf::Vertex vertices[3] = {};
			{
				vertices[0].position = pos;
				vertices[1].position = pos1;
				vertices[2].position = pos2;
			}

			{
				sf::Color color = sfColor(i.color);
				vertices[0].color = color;
				vertices[1].color = sf::Color(color.r, color.g, color.b, 0);
				vertices[2].color = sf::Color(color.r, color.g, color.b, 0);
			}

			{
				vertices[0].texCoords = sf::Vector2f(0,0);
				vertices[1].texCoords = sf::Vector2f(1,0);
				vertices[2].texCoords = sf::Vector2f(0,1);
			}

			renderTarget.draw(vertices, 3, sf::Triangles, states);
		}
		else if (i.type == GuiRenderInfoType::Rect)
		{
			float r = i.w * g_resolution.y;
			bool filled = i.filled;
			sf::Vector2f size(i.w * g_resolution.y, i.h * g_resolution.y);
			sf::RenderStates states = sf::RenderStates::Default;

			const int polyCount = 5;
			sf::Vertex vertices[polyCount] = {};
			{
				float left = pos.x;
				float right = pos.x + size.x;
				float top = pos.y;
				float bottom = pos.y + size.y;
				vertices[0].position = sf::Vector2f(left, top);
				vertices[1].position = sf::Vector2f(right, top);
				vertices[2].position = sf::Vector2f(right, bottom);
				vertices[3].position = sf::Vector2f(left, bottom);
				vertices[4].position = sf::Vector2f(left, top);
			}

			{
				sf::Color color = sfColor(i.color);
				vertices[0].color = color;
				vertices[1].color = color;
				vertices[2].color = color;
				vertices[3].color = color;
				vertices[4].color = color;
			}

			if (filled)
				renderTarget.draw(vertices, polyCount, sf::TriangleFan, states);
			else
				renderTarget.draw(vertices, polyCount, sf::LineStrip, states);
		}
	}
}
