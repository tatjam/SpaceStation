#include <SFML/Graphics.hpp>

#include "server/Server.h"
#include "client/Client.h"


#include "shared/SquirrelScript.h"
#include "server/FlowSimulation.h"



int main()
{
	FlowSimulation flow;
	flow.create(125, 125);
	flow.map[1 * flow.width + 1] = 100000;
	//flow.map[12 * flow.width + 5] = 1;

	for (int i = 0; i < 15; i++)
	{
		flow.map[i * flow.width + 8] = -2;
		flow.map[i * flow.width + 2] = -2;
		/*flow.map[2 * flow.width + i] = -2;
		flow.map[8 * flow.width + i] = -2;
		flow.map[13 * flow.width + i] = -2;*/
	}

	flow.map[8 * flow.width + 7] = 0.0f;

	//flow.map[8 * flow.width + 8] = 9.0f;

	/*while (true)
	{

		flow.termDraw();
		flow.iterate();

		printf("\n\n\n\n\n\n");

		getchar();
	}*/

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


		flow.iterate();
		flow.draw(client.win, 100.0f);
		client.display();

		dt = dtc.restart().asSeconds();
		printf("FPS: %f\n", 1 / dt);

	}

	return 0;
}