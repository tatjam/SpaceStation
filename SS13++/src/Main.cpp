#include <SFML/Graphics.hpp>

#include "server/Server.h"
#include "client/Client.h"


#include "shared/SquirrelScript.h"

#include "shared/AssetManager.h"
#include "shared/Tilemap.h"

int main()
{

	AssetManager assetManager = AssetManager("res/");
	assetManager.getTex("test.png");

	assetManager.loadTiles("res/data/tiles.json");

	Tilemap tilemap = Tilemap(10, 10, &assetManager);
	tilemap.get(5, 5)->id = "floor";
	tilemap.get(6, 5)->id = "floor";
	tilemap.get(5, 6)->id = "floor";
	tilemap.get(6, 6)->id = "floor";
	tilemap.get(5, 7)->id = "tubes";
	tilemap.get(6, 7)->id = "tubes";
	tilemap.get(5, 8)->id = "tubes";
	tilemap.get(6, 8)->id = "tubes";

	Server server = Server();
	Client client = Client();

	NetSender serverNet;
	serverNet.ip = sf::IpAddress::getLocalAddress();
	serverNet.port = server.socket->socket->getLocalPort();

	sf::Clock dtc = sf::Clock();
	float dt = 0.0f;

	float timer = 0.0f;

	while (client.win->isOpen())
	{
		if (client.connected == false)
		{
			client.connect(serverNet);
		}
		server.update(dt);
		client.update(dt);

		client.win->clear();
		tilemap.render(client.win, sf::Vector2f(-5.0f, -128.0f), sf::Vector2f(2, 2));
		client.display();

		dt = dtc.restart().asSeconds();

		timer += dt;

		if (timer > 0.5f)
		{
			client.win->setTitle("FPS: " + std::to_string(1.0f / dt));
			timer = 0.0f;
		}

		//rintf("FPS: %f\n", 1 / dt);

	}

	return 0;
}