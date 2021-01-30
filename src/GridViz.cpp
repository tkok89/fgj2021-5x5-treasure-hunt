#include "GridViz.h"

#include "Grid.h"
#include "GuiRendering.h"
#include "GuiRenderInfo.h"
#include "Resources.h"

void GridViz::render(Grid &grid, sf::Vector2f pos, sf::Vector2f size, sf::Vector2i moveThisRow, float moveAmount)
{
	sf::Vector2f tileSize = size / float(GridSize);

	ForGrid(tile)
	{
		sf::Texture &texture = Resources::getResources().tileTextures[tile->type];
		float x = pos.x + tileX * tileSize.x + (tileY == moveThisRow.y ? moveAmount : 0.f);
		float y = pos.y + tileY * tileSize.y + (tileX == moveThisRow.x ? moveAmount : 0.f);
		GuiRendering::image(&texture, x, y, tileSize.x, tileSize.y);
	}
}