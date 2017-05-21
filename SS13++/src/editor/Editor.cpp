#include "Editor.h"

#define NOC_FILE_DIALOG_IMPLEMENTATION
#include "../dep/noc_file_dialog.h"



std::string Editor::getFile(std::string path)
{
	std::string buff = "";

	for (int i = path.size(); i > 0; i--)
	{
		if (path[i] == '/' || path[i] == '\\')
		{
			break;
		}
		buff += path[i];
	}

	std::reverse(buff.begin(), buff.end());

	return buff;
}

void Editor::resize(sf::Vector2f newSize)
{
	view = sf::View(sf::FloatRect(0, 0, newSize.x, newSize.y));
	//view.setCenter(focus);
	view.setSize(sf::Vector2f(newSize.x, newSize.y));
	view.zoom(zoom);
}

bool Editor::uiTileButton(TileInfo* t, float scale)
{
	bool click = ImGui::ImageButton(*t->tex, sf::Vector2f(TILE_SIZE * scale, TILE_SIZE * scale));
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("%s ('%s')    ", t->name.c_str(), t->cname.c_str());
		ImGui::Separator();
		ImGui::TextWrapped("%s", t->desc.c_str());
		ImGui::EndTooltip();
	}

	return click;
}

void Editor::doInput(float dt)
{
	bool shiftDown = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
	// Camera handling

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		if (shiftDown) { zoom += ZOOM_FACTOR * ZOOM_SHIFT * dt; }
		else { zoom += ZOOM_FACTOR * dt; }
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
}

void Editor::save(std::string path)
{
	std::ofstream fout(path, std::ofstream::out | std::ofstream::trunc);

	if (fout.bad())
	{
		return;
	}

	fout << map->serialize();

	fout.close();
}

void Editor::load(std::string path)
{
	std::ifstream fin(path);

	if (fin.bad())
	{
		return;
	}

	std::string str((std::istreambuf_iterator<char>(fin)),
		std::istreambuf_iterator<char>());

	map->deserialize(str);

	fin.close();
}

void Editor::uiMenuBar()
{
	ImGui::BeginMainMenuBar();
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("New");
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				const char* ff = noc_file_dialog_open(NOC_FILE_DIALOG_OPEN, "tmf\0*.tmf", "./res", "");
				if (ff == NULL)
				{
					printf("Loading from no place\n");
				}
				else
				{
					printf("Loading: %s\n", ff);
					load(std::string(ff));
				}
			}
			if (ImGui::MenuItem("Save", "CTRL+S"))
			{
				if (prevSave == "")
				{
					const char* ff = noc_file_dialog_open(NOC_FILE_DIALOG_SAVE, "tmf\0*.tmf", "./res", "");
					if (ff == NULL)
					{
						printf("Saving to no place\n");
					}
					else
					{
						prevSave = std::string(ff);
						printf("File: %s\n", prevSave.c_str());
						printf("Saving to: %s\n", ff);
						save(std::string(ff));
					}
				}
				else
				{
					save(prevSave);
				}
			}
			if (ImGui::MenuItem("Save As..", "SHIFT+S"))
			{
				const char* ff = noc_file_dialog_open(NOC_FILE_DIALOG_SAVE, "tmf\0*.tmf", "./res", "");
				if (ff == NULL)
				{
					printf("Saving to no place\n");
				}
				else
				{
					prevSave = std::string(ff);
					printf("File: %s\n", prevSave.c_str());
					printf("Saving to: %s\n", ff);
					save(std::string(ff));
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

}

void Editor::update(float dt)
{

	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		map->clearLight(0, 0, map->width, map->height);
		map->doLight(tX, tY, sf::Color::White, 6, 1.0f);
	}
	else
	{
		map->clearLight(0, 0, map->width, map->height, sf::Color::White);
	}

	doInput(dt);

	uiMenuBar();

	float scale = 2.0f;
	int maxX = 3;

	ImGui::Begin("Tiles");

	bool eraserSel = selected == NULL;

	if (eraserSel)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8, 0.8, 0.8, 0.5));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.5));
	}

	if (eraseTop)
	{
		if (ImGui::Button("Erase Top", ImVec2(-1, 0)))
		{
			if (eraserSel)
			{
				eraseTop = false;
			}
			selected = NULL;
		}
	}
	else
	{
		if (ImGui::Button("Erase Bottom", ImVec2(-1, 0)))
		{
			if (eraserSel)
			{
				eraseTop = true;
			}
			selected = NULL;
		}
	}

	if (eraserSel)
	{
		ImGui::PopStyleColor(2);
	}

	if (!showTop)
	{
		if (ImGui::Button("Bottom", ImVec2(-1, 0)))
		{
			showTop = true;
		}
	}
	else
	{
		if (ImGui::Button("Top", ImVec2(-1, 0)))
		{
			showTop = false;
		}
	}

	ImGui::BeginChild("TileList", ImVec2(0, 0), true);

	int xoff = 0;

	for (std::unordered_map<std::string, TileInfo>::iterator it = assets->tiles.begin(); it != assets->tiles.end(); ++it)
	{
		bool styled = selected == &it->second;

		if (styled)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8, 0.8, 0.8, 0.5));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.5));
		}
		if (it->second.top == showTop)
		{
			if (uiTileButton(&it->second, scale))
			{
				selected = &it->second;
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

		if (styled)
		{
			ImGui::PopStyleColor(2);
		}
	}

	ImGui::EndChild();

	ImGui::End();

	if (!ImGui::GetIO().WantCaptureMouse)
	{

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (selected == NULL)
			{
				if (tX >= 0 && tY >= 0 && tX < map->width && tY < map->height)
				{
					int i = tY * map->width + tX;

					if (eraseTop)
					{
						map->tiles[i].top = NULL;
					}
					else
					{
						map->tiles[i].bot = NULL;
					}
				}
			}
			else
			{
				if (tX >= 0 && tY >= 0 && tX < map->width && tY < map->height)
				{
					int i = tY * map->width + tX;

					if (selected->top)
					{
						map->tiles[i].top = selected;
					}
					else
					{
						map->tiles[i].bot = selected;
					}
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

	// Size rectangles

	sf::RectangleShape r = sf::RectangleShape(sf::Vector2f(map->width * TILE_SIZE, map->height * TILE_SIZE));
	r.setFillColor(sf::Color::Black);
	r.setOutlineThickness(4.0f);
	r.setOutlineColor(sf::Color(128, 128, 128, 128));

	resize((sf::Vector2f)win->getSize());
	win->setView(view);

	r.setPosition(-focus);

	win->draw(r);

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
