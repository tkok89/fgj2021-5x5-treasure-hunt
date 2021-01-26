#pragma once

#include <string.h>

enum class GuiRenderInfoType
{
	Image,
	Text,
};

struct GuiRenderInfo
{
	GuiRenderInfo()
	{
		memset(this, 0, sizeof(GuiRenderInfo));
	}

	GuiRenderInfoType type;
	float x, y, w, h;

	struct
	{
		float r, g, b, a;
	} color;

	union
	{
		struct
		{
			const void *image;
			float uvX, uvY, uvW, uvH;
		};
		struct
		{
			const char *strPtr;
			int strLen;

			void *font;
			float fontHeight;

			float outline;
			struct
			{
				float r, g, b, a;
			} outlineColor;
		};
	};
};
