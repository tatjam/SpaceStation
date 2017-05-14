#include "Server.h"

void Server::update(float dt)
{
	RMessage* r = socket->update(dt);
	if(r != NULL)
	{
		//printf("Received: %s\n", NetUtil::toString(r->command).c_str());
		if(strcmp(r->command.command.c_str(), "CONNECT") == 0)
		{
			printf("Connect request received! (%s)\n", r->command.args[0].c_str());
			NetCommand reply;
			reply.command = "CONNECT_REPLY";
			reply.args.push_back("ACCEPTED");
			socket->send(reply, r->addr, r->port);
		}
	}

}

Server::Server()
{
	socket = new RSocket();
	socket->bind(1234);
}


Server::~Server()
{
}
