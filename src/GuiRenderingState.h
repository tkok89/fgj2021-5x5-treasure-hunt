#pragma once

#include "GuiRenderInfo.h"
#include <vector>

struct ClipRect
{
	ClipRect() = delete;
	ClipRect(float x, float y, float w, float h)
		: x(x), y(y), w(w), h(h)
	{
	}

	float x, y, w, h;
};

static const ClipRect getAntiClipRect()
{
	return { -100000, -100000, 300000, 300000 };
}

struct GuiRenderingState
{
	std::vector<GuiRenderInfo> renderInfos;
	ClipRect currentClipRect = getAntiClipRect();

	void pushGuiRenderInfo(GuiRenderInfo guiRenderInfo);
	void setClipRect(ClipRect clipRect) { currentClipRect = clipRect;  }
};
