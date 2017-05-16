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

	Tilemap tilemap = Tilemap(10, 10, &assetManager);
	tilemap.get(5, 5)->id = "asd";

	Server server = Server();
	Client client = Client();

	NetSender serverNet;
	serverNet.ip = sf::IpAddress::getLocalAddress();
	serverNet.port = server.socket->socket->getLocalPort();

	sf::Clock dtc = sf::Clock();
	float dt = 0.0f;

	while (client.win->isOpen())
	{
		if (client.connected == false)
		{
			client.connect(serverNet);
		}
		server.update(dt);
		client.update(dt);

		client.win->clear();
		tilemap.render(client.win);
		client.display();

		dt = dtc.restart().asSeconds();
		//rintf("FPS: %f\n", 1 / dt);

	}

	return 0;
}