#include "Menu.h"

#include "GuiRendering.h"
#include "GuiRenderingState.h"
#include "SfmlGuiRendering.h"
#include "GuiRenderInfo.h"
#include <assert.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <SFML/Graphics/Transformable.hpp>

struct WidgetId
{
	unsigned depth = 0;
	unsigned valueStack[10] = { 0 };
};

static bool operator==(const WidgetId &a, const WidgetId &b)
{
	if (a.depth != b.depth)
		return false;

	for (unsigned i = 0; i <= a.depth; ++i)
	{
		if (a.valueStack[i] == b.valueStack[i])
			continue;

		return false;
	}

	return true;
}

struct WidgetIdHasher
{
	size_t operator()(const WidgetId v) const noexcept
	{
		size_t seed = 0x9e3779b9;
		for (size_t i = 0; i <= v.depth; ++i)
		{
			// Roughly boost::hash_combine
			seed ^= v.valueStack[i] * 0x9e3779b9 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	}
};

struct WidgetTransform
{
	sf::Transform transform;
};

struct WidgetState
{
	bool pressed = false;
	bool hovered = false;
	bool released = false;

	int textCaret = 0;
	int textSelectionStart = 0;
	int textSelectionPivot = 0;
	int textSelectionEnd = 0;
};

struct GlobalWidgetState
{
	WidgetId currentWidgetId;
	std::unordered_map<WidgetId, std::string, WidgetIdHasher> names;
	std::unordered_map<WidgetId, WidgetTransform, WidgetIdHasher> transforms;
	std::unordered_map<WidgetId, WidgetState, WidgetIdHasher> states;
	std::vector<sf::Transform> transformStack;
	sf::Transform currentTransform;
};

static GlobalWidgetState g_globalWidgetState;

static void startWidget(const char *name = nullptr)
{
	assert(g_globalWidgetState.currentWidgetId.depth < 10);

	g_globalWidgetState.currentWidgetId.valueStack[g_globalWidgetState.currentWidgetId.depth] += 1;
	g_globalWidgetState.currentWidgetId.depth += 1;
	g_globalWidgetState.currentWidgetId.depth = g_globalWidgetState.currentWidgetId.depth;

	if (name && g_globalWidgetState.names[g_globalWidgetState.currentWidgetId] != name)
		g_globalWidgetState.names[g_globalWidgetState.currentWidgetId] = name;

	auto it = g_globalWidgetState.transforms.find(g_globalWidgetState.currentWidgetId);
	if (it != g_globalWidgetState.transforms.end())
	{
		g_globalWidgetState.currentTransform *= it->second.transform;
		g_globalWidgetState.transformStack.push_back(g_globalWidgetState.currentTransform);
	}
	else
	{
		g_globalWidgetState.transformStack.push_back(g_globalWidgetState.currentTransform);
	}
}

static void endWidget(const char *name = nullptr)
{
	assert(g_globalWidgetState.currentWidgetId.depth > 0);
	g_globalWidgetState.currentWidgetId.valueStack[g_globalWidgetState.currentWidgetId.depth] = 0;

	assert(!name || g_globalWidgetState.names[g_globalWidgetState.currentWidgetId] == name);

	g_globalWidgetState.currentWidgetId.depth -= 1;

	assert(!g_globalWidgetState.transformStack.empty());
	g_globalWidgetState.transformStack.pop_back();
	if (g_globalWidgetState.transformStack.empty())
		g_globalWidgetState.currentTransform = sf::Transform();
	else
		g_globalWidgetState.currentTransform = g_globalWidgetState.transformStack.back();
}

static void setWidgetTransform(sf::Transform transform)
{
	assert(!g_globalWidgetState.transformStack.empty());
	g_globalWidgetState.transformStack.pop_back();
	g_globalWidgetState.currentTransform = g_globalWidgetState.transformStack.back() * transform;
	g_globalWidgetState.transformStack.push_back(g_globalWidgetState.currentTransform);
}

static const sf::Transform &getWidgetTransform()
{
	return g_globalWidgetState.currentTransform;
}

static void updateWidgets()
{
	assert(g_globalWidgetState.currentWidgetId.depth == 0);
	g_globalWidgetState.currentWidgetId = {};
}

struct MenuState
{

};

void Menu::render(void *&menuStatePtr)
{
	if (!menuStatePtr)
	{
		menuStatePtr = new MenuState();
	}

	MenuState &state = *(MenuState*)menuStatePtr;

	startWidget("Menu");

	startWidget();
	startWidget();

	startWidget("Nappi");
	endWidget("Nappi");

	endWidget();
	endWidget();

	startWidget();
	endWidget();

	startWidget();

	startWidget();
	startWidget();
	endWidget();
	endWidget();

	startWidget();
	endWidget();

	endWidget();

	endWidget("Menu");

	updateWidgets();
}
