#include "Client.h"

void Client::update(float dt)
{
	sf::Event e;
	while (win->pollEvent(e))
	{
		// Process event
		if (e.type == sf::Event::Closed)
		{
			win->close();
		}
	}

	timer += dt;

	RMessage* msg = socket->update(dt);
	if (connected)
	{
		
	}
	else
	{
		if (connecting)
		{
			if (msg != NULL)
			{
				if (strcmp(msg->command.command.c_str(), "CONNECT_REPLY") == 0)
				{
					if (strcmp(msg->command.args[0].c_str(), "ACCEPTED") == 0)
					{
						printf("Accepted!");
						connecting = false;
						connected = true;
					}
					else
					{
						printf("Connection denied, reason: %s\n", msg->command.args[0].c_str());
					}
				}
			}
			if (timer > 1.0f)
			{
				NetCommand cmd = NetCommand();
				cmd.command = "CONNECT";
				cmd.args.push_back("Tatjam");
				socket->send(cmd, server.ip, server.port);
				timer = 0.0f;
				printf("Sent connect request!\n");
			}
		}
	}
}

void Client::connect(NetSender server)
{
	this->server = server;
	this->connecting = true;
}


void Client::display()
{
	win->display();
}

Client::Client()
{

	win = new sf::RenderWindow(sf::VideoMode(512, 512), "Hello World");

	cClock = sf::Clock();

	socket = new RSocket();

	socket->bind(5555);

}


Client::~Client()
{
}
