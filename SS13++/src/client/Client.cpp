#include "Client.h"

void Client::update(float dt)
{
	timer += dt;
	if (timer > 0.5f)
	{
		socket->send("HELLO WORLD", 12, server.ip, server.port);
		timer = 0.0f;
	}
}

void Client::connect(NetSender server)
{
	if (!connected)
	{
		this->server = server;

		bool first = true;

		char receive[PACKET_SIZE];
		std::size_t receivedSize;

		sf::IpAddress in;
		ushort port;

		if (cClock.getElapsedTime().asSeconds() > 0.5f)
		{
			NetCommand cmd;
			cmd.command = "CONNECT";
			cmd.args.push_back("Tatjam");

			printf("Sending: %s\n", NetUtil::toString(cmd).c_str());

			printf("Trying connect...\n");

			cmd.id = id;

			NetUtil::sendMessage(socket, cmd, server.ip, server.port);

			id++;

			first = false;
			cClock.restart();
		}

		if (socket->receive(receive, PACKET_SIZE, receivedSize, in, port) != sf::Socket::NotReady)
		{
			NetCommand cmd = NetUtil::breakMessage(receive, receivedSize);
			printf("Received: %s\n", NetUtil::toString(cmd).c_str());
			if (cmd.command == "CONNECT_REPLY" && cmd.args.size() == 1)
			{
				if (cmd.args[0] == "ACCEPT")
				{
					printf("Accepted!\n");
					connected = true;
				}
				else
				{
					return;
				}
			}
		}
	}

}

Client::Client()
{
	cClock = sf::Clock();

	socket = new sf::UdpSocket();

	if (socket->bind(5555) != sf::Socket::Done)
	{
		printf("Could not create client socket!\n");
	}

	socket->setBlocking(false);
}


Client::~Client()
{
}
