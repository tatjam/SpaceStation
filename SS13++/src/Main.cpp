#include <SFML/Graphics.hpp>

#include "server/Server.h"
#include "client/Client.h"


#include "shared/SquirrelScript.h"

#include "shared/AssetManager.h"


int main()
{

	AssetManager assetManager = AssetManager("res/");
	assetManager.getTex("test.png");

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

		client.display();

		dt = dtc.restart().asSeconds();
		//rintf("FPS: %f\n", 1 / dt);

	}

	return 0;
}