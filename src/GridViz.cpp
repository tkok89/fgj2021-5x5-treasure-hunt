#include"GridViz.h"

#include "Grid.h"
#include "GuiRendering.h"
#include "GuiRenderInfo.h"
#include "Resources.h"

void GridViz::render(Grid &grid, sf::Vector2f pos, sf::Vector2f size)
{
	sf::Vector2f tileSize = size / float(GridSize);

	GuiRenderInfo guiRenderInfo;
	guiRenderInfo.type = GuiRenderInfoType::Image;
	guiRenderInfo.w = tileSize.x;
	guiRenderInfo.h = tileSize.y;
	guiRenderInfo.uvX = 0;
	guiRenderInfo.uvY = 0;
	guiRenderInfo.uvW = 1;
	guiRenderInfo.uvH = 1;
	guiRenderInfo.color.r = 1;
	guiRenderInfo.color.g = 1;
	guiRenderInfo.color.b = 1;
	guiRenderInfo.color.a = 1;
	ForGrid(ii, tile)
	{
		sf::Texture &texture = Resources::getResources().tileTextures[tile->type];
		guiRenderInfo.x = pos.x + x * tileSize.x;
		guiRenderInfo.y = pos.y + y * tileSize.y;
		guiRenderInfo.image = &texture;

		GuiRendering::render(guiRenderInfo);
	}
}