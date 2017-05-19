#pragma once
#include <SFML/Graphics.hpp>

#include "../shared/Tilemap.h"

#include "../dep/imgui/imgui.h"
#include "../dep/imgui/imgui-SFML.h"

// Standalone editor, probably a separate executable
// but can be launched from a client
// Uses ImGui, please call the platform specific functions yourself!
class Editor
{
public:

	Tilemap* map;

	void update(float dt);

	void render(sf::RenderWindow* win);

	Editor(Tilemap* toEdit);
	~Editor();
};

