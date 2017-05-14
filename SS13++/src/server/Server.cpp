#include "Server.h"

void Server::update(float dt)
{
	char data[258];
	std::size_t received;

	sf::IpAddress remoteA;
	ushort remoteP;
	if (socket->receive(data, 256, received, remoteA, remoteP) != sf::Socket::NotReady)
	{
		NetCommand cmd = NetUtil::breakMessage(data, received);
		printf("Received packet! Content: (%s)\n", data);
		printf("Command: %s\n", NetUtil::toString(cmd).c_str());
		printf("Sender IP: (%s) Port: (%i)\n", remoteA.toString().c_str(), remoteP);
		if (cmd.command == "CONNECT" && cmd.args.size() == 1)
		{
			printf("Connect request received! (%s)\n", cmd.args[0].c_str());
			NetCommand reply;
			reply.command = "CONNECT_REPLY";
			reply.args.push_back("ACCEPT");
			reply.id = id;
			NetUtil::sendMessage(socket, reply, remoteA, remoteP);
			id++;
		}
	}
}

Server::Server()
{
	socket = new sf::UdpSocket();
	if (socket->bind(1234) != sf::Socket::Done)
	{
		printf("Could not create server socket!\n");
	}

	socket->setBlocking(false);
}


Server::~Server()
{
}
