#pragma once
#include <SFML/Graphics.hpp>

#include "../shared/Tilemap.h"
#include "../shared/AssetManager.h"

#include "../dep/imgui/imgui.h"
#include "../dep/imgui/imgui-SFML.h"

#include "../dep/noc_file_dialog.h"

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

	std::string prevSave = "";

	sf::Vector2f sizebase = sf::Vector2f(512, 512);

	sf::Vector2f focus = sf::Vector2f(0, 0);

	sf::View view;

	AssetManager* assets;

	Tilemap* map;
	int tX, tY;

	TileInfo* selected = NULL;

	bool eraseTop = false;

	bool showTop = false;


	sf::RectangleShape cursor;

	std::string getFile(std::string path);
	
	float zoom = 1.0f;

	void resize(sf::Vector2f newSize);

	bool uiTileButton(TileInfo* t, float scale);

	void load(std::string path);

	void save(std::string path);

	void doInput(float dt);

	void uiMenuBar();

	void update(float dt);

	void render(sf::RenderWindow* win);

	Editor(Tilemap* toEdit, AssetManager* assets);
	~Editor();
};

