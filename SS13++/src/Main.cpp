#include <SFML/Graphics.hpp>

#include "server/Server.h"
#include "client/Client.h"


#include "shared/SquirrelScript.h"

#include "shared/AssetManager.h"
#include "shared/Tilemap.h"

#include "editor/Editor.h"

int main()
{

	AssetManager assetManager = AssetManager("res/");
	assetManager.getTex("test.png");

	assetManager.loadTiles("res/data/tiles.json");

	Tilemap tilemap = Tilemap(30, 30, &assetManager);

	tilemap.tiles[0].bot = assetManager.getTile("floor");
	tilemap.tiles[0].top = assetManager.getTile("tubes");
	tilemap.tiles[0].botHealth = 100.0f;
	tilemap.tiles[0].topHealth = 100.0f;

	sf::RenderWindow* win = new sf::RenderWindow(sf::VideoMode(768, 768), "Editor Window");

	ImGui::SFML::Init(*win);

	Editor editor = Editor(&tilemap, &assetManager);

	sf::Event ev;

	sf::Clock dtc;
	sf::Time dtt;
	float dt = 0.0f;

	editor.sizebase = (sf::Vector2f)win->getSize();
	editor.resize(editor.sizebase);

	while (win->isOpen())
	{
		while (win->pollEvent(ev))
		{
			ImGui::SFML::ProcessEvent(ev);

			if (ev.type == sf::Event::Closed)
			{
				win->close();
			}
			else if (ev.type == sf::Event::Resized)
			{
				editor.sizebase = sf::Vector2f(ev.size.width, ev.size.height);
				editor.resize(sf::Vector2f(ev.size.width, ev.size.height));
			}
		}

		ImGui::SFML::Update(*win, dtt);

		editor.update(dt);

		win->clear();

		editor.render(win);

		win->pushGLStates();
		ImGui::Render();
		win->popGLStates();

		win->display();



		dtt = dtc.restart();
		dt = dtt.asSeconds();

		win->setTitle("FPS: " + std::to_string(1.0f / dt));

	}




	ImGui::SFML::Shutdown();

	return 0;
}