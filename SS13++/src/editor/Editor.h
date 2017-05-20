#pragma once
#include <SFML/Graphics.hpp>

#include "../shared/Tilemap.h"
#include "../shared/AssetManager.h"

#include "../dep/imgui/imgui.h"
#include "../dep/imgui/imgui-SFML.h"

#define ZOOM_FACTOR 1.0f
#define ZOOM_SHIFT 1.5f

#define MOVE_SPEED 270.0f
#define ZOOM_SHIFT 3.0f

// Standalone editor, probably a separate executable
// but can be launched from a client
// Uses ImGui, please call the platform specific functions yourself!
class Editor
{
public:

	sf::Vector2f sizebase = sf::Vector2f(512, 512);

	sf::Vector2f focus = sf::Vector2f(0, 0);

	sf::View view;

	AssetManager* assets;

	Tilemap* map;
	int tX, tY;

	std::string selected = "";


	sf::RectangleShape cursor;

	float zoom = 1.0f;

	void resize(sf::Vector2f newSize);

	void update(float dt);

	void render(sf::RenderWindow* win);

	Editor(Tilemap* toEdit, AssetManager* assets);
	~Editor();
};

