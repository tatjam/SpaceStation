#include <SFML/Graphics.hpp>

#include "server/Server.h"
#include "client/Client.h"

int main()
{
	NetCommand cmd;
	cmd.command = "SAY";
	cmd.args.push_back("Well, that was interesting lol!");
	std::size_t outsize;
	sf::Clock measurer;
	measurer.restart();
	NetUtil::formMessage(cmd, &outsize);
	float taken = measurer.restart().asSeconds();
	printf("Taken: %f\n", taken);

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

		dt = dtc.restart().asSeconds();
	}

	return 0;
}