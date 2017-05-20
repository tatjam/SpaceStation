#include "Editor.h"



void Editor::resize(sf::Vector2f newSize)
{
	view = sf::View(sf::FloatRect(0, 0, newSize.x, newSize.y));
	//view.setCenter(focus);
	view.setSize(sf::Vector2f(newSize.x, newSize.y));
	view.zoom(zoom);
}

void Editor::update(float dt)
{

	bool shiftDown = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
	// Camera handling

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		if (shiftDown){zoom += ZOOM_FACTOR * ZOOM_SHIFT * dt;}
		else{zoom += ZOOM_FACTOR * dt;}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
	{
		if (shiftDown) { zoom -= ZOOM_FACTOR * ZOOM_SHIFT * dt; }
		else { zoom -= ZOOM_FACTOR * dt; }
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		if (shiftDown) { focus.y -= MOVE_SPEED * ZOOM_SHIFT * dt; }
		else { focus.y -= MOVE_SPEED * dt; }
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		if (shiftDown) { focus.x -= MOVE_SPEED * ZOOM_SHIFT * dt; }
		else { focus.x -= MOVE_SPEED * dt; }
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		if (shiftDown) { focus.y += MOVE_SPEED * ZOOM_SHIFT * dt; }
		else { focus.y += MOVE_SPEED * dt; }
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (shiftDown) { focus.x += MOVE_SPEED * ZOOM_SHIFT * dt; }
		else { focus.x += MOVE_SPEED * dt; }
	}
	float scale = 2.0f;

	int maxX = 3;

	ImGui::Begin("Tiles");

	if (ImGui::Button("Eraser", ImVec2(-1, 0)))
	{
		selected = "";
	}

	ImGui::BeginChild("TileList", ImVec2(0, 0), true);

	int xoff = 0;

	for (std::unordered_map<std::string, TileInfo>::iterator it = assets->tiles.begin(); it != assets->tiles.end(); ++it)
	{
		bool styled = selected == it->first;

		if (styled)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8, 0.8, 0.8, 0.5));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.5));
		}

		if (ImGui::ImageButton(*it->second.top, sf::Vector2f(scale * TILE_SIZE, scale * TILE_SIZE)))
		{
			selected = it->first;
		}

		if (styled)
		{
			ImGui::PopStyleColor(2);
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("%s ('%s')    ", it->second.name.c_str(), it->first.c_str());
			ImGui::Separator();
			ImGui::TextWrapped("%s", it->second.desc.c_str());
			ImGui::EndTooltip();
		}

		if (xoff >= maxX - 1)
		{
			xoff = 0;
		}
		else
		{
			ImGui::SameLine();
			xoff++;
		}
	}

	ImGui::EndChild();

	ImGui::End();

	if (!ImGui::GetIO().WantCaptureMouse)
	{

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (selected == "DELETER" || selected == "")
			{
				if (tX >= 0 && tY >= 0 && tX < map->width && tY < map->height)
				{
					int i = tY * map->width + tX;

					map->tiles[i].id = "";
				}
			}
			else
			{
				if (tX >= 0 && tY >= 0 && tX < map->width && tY < map->height)
				{
					int i = tY * map->width + tX;

					map->tiles[i].id = selected;
				}
			}
		}
		cursor.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	}
	else
	{
		cursor.setSize(sf::Vector2f(0, 0));
	}
	

}

void Editor::render(sf::RenderWindow* win)
{
	resize((sf::Vector2f)win->getSize());
	win->setView(view);

	//sizebase = (sf::Vector2f)win->getSize();

	sf::Vector2i mousePos = sf::Mouse::getPosition(*win);
	sf::Vector2f realPos = win->mapPixelToCoords(mousePos, view);
	realPos += focus;
	int tileX = realPos.x - (int)realPos.x % TILE_SIZE;
	int tileY = realPos.y - (int)realPos.y % TILE_SIZE;

	// Fixes negative offsets (kind of)
	// As you cannot edit tiles at negative positions
	// this is pretty useless! Anyway just in case the map
	// system allows negative coordinates this exists!

	if (realPos.x < 0) { 
		tileX -= TILE_SIZE; 
	}
	if (realPos.y < 0) { 
		tileY -= TILE_SIZE; 
	}

	cursor.setPosition(tileX - focus.x, tileY - focus.y);

	tX = tileX/TILE_SIZE; tY = tileY/TILE_SIZE;

	map->render(win, -focus);
	win->draw(cursor);
}

Editor::Editor(Tilemap* t, AssetManager* assets)
{
	this->map = t;
	this->assets = assets;
}


Editor::~Editor()
{
}
