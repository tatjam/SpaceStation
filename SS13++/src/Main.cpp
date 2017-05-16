#include <SFML/Graphics.hpp>

#include "server/Server.h"
#include "client/Client.h"


#include "shared/SquirrelScript.h"
#include "server/FlowSimulation.h"



int main()
{
	FlowSimulation flow;
	flow.create(15, 15);
	flow.map[5 * flow.width + 5] = 9.0f;

	for (int i = 0; i < 15; i++)
	{
		flow.map[i * flow.width + 8] = -2.0f;
		flow.map[i * flow.width + 2] = -2.0f;
		flow.map[2 * flow.width + i] = -2.0f;
		flow.map[8 * flow.width + i] = -2.0f;
		flow.map[13 * flow.width + i] = -2.0f;
	}

	flow.map[8 * flow.width + 7] = 0.0f;

	//flow.map[8 * flow.width + 8] = 9.0f;

	while (true)
	{

		flow.draw();
		flow.iterate();

		printf("\n\n\n\n\n\n");

		getchar();
	}

	Server server = Server();
	Client client = Client();

	NetSender serverNet;
	serverNet.ip = sf::IpAddress::getLocalAddress();
	serverNet.port = server.socket->socket->getLocalPort();

	sf::Clock dtc = sf::Clock();
	float dt = 0.0f;

	while (true)
	{
		if (client.connected == false)
		{
			client.connect(serverNet);
		}
		server.update(dt);
		client.update(dt);

		client.display();

		dt = dtc.restart().asSeconds();
	}

	return 0;
}