#pragma once
#include <SFML/Network.hpp>
#include "../server/Server.h"



class Client
{
private:

	sf::Clock cClock;
	ncid id;

public:



	bool connected = false;

	std::string name;

	float timer = 0.0f;

	NetSender server;

	sf::UdpSocket* socket;

	void update(float dt);

	void connect(NetSender server);

	Client();
	~Client();
};

