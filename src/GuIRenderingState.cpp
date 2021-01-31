#include "GuiRenderingState.h"

static float min(float a, float b)
{
	return a <= b ? a : b;
}

static float max(float a, float b)
{
	return a >= b ? a : b;
}

static float clamp(float a, float low, float high)
{
	return min(max(a, low), high);
}

static bool clip(float &x, float &y, float &w, float &h, float &u, float &v, float &uW, float &vH, const ClipRect currentClipRect)
{
	float x0 = x;
	float y0 = y;
	float x1 = x + w;
	float y1 = y + h;
	float w_ = w;
	float h_ = h;
	float u0 = u;
	float v0 = v;
	float u1 = u + uW;
	float v1 = v + vH;
	float cx0 = currentClipRect.x;
	float cy0 = currentClipRect.y;
	float cx1 = currentClipRect.x + currentClipRect.w;
	float cy1 = currentClipRect.y + currentClipRect.h;

	x = max(x0, cx0);
	y = max(y0, cy0);
	w = min(x1, cx1) - x;
	h = min(y1, cy1) - y;

	u = u0 + (x - x0) / w_ * uW;
	v = v0 + (y - y0) / h_ * vH;
	uW = u1 - u - (x1 - (x + w)) / w_ * uW;
	vH = v1 - v - (y1 - (y + h)) / h_ * vH;

	return true;
}

void GuiRenderingState::pushGuiRenderInfo(GuiRenderInfo i)
{
	if (i.type == GuiRenderInfoType::Image)
	{
		if (clip(i.x, i.y, i.w, i.h, i.uvX, i.uvY, i.uvW, i.uvH, currentClipRect))
		{
			renderInfos.push_back(i);
		}
	}
	else if (i.type == GuiRenderInfoType::Text)
	{
		// TODO: Clipping for text
		renderInfos.push_back(i);
	}
	else if (i.type == GuiRenderInfoType::Line)
	{
		// TODO: Clipping for lines
		renderInfos.push_back(i);
	}
	else if (i.type == GuiRenderInfoType::Circle)
	{
		renderInfos.push_back(i);
	}
	else if (i.type == GuiRenderInfoType::Triangle)
	{
		renderInfos.push_back(i);
	}
	else if (i.type == GuiRenderInfoType::Rect)
	{
		// TODO: Clip
		renderInfos.push_back(i);
	}
}
