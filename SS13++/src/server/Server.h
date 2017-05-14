#pragma once

#define CONNECT_PACKET "CONN"
#define JOIN_PACKET "ACCEPT"

#include <map>
#include <SFML/Network.hpp>
#include "../util/NetUtil.h"

#include "../util/RSocket.h"

typedef unsigned short ushort;

struct NetSender
{
	sf::IpAddress ip;
	ushort port;
};

class Server
{

private:
	ncid id;

public:

	RSocket* socket;

	std::map<NetSender, sf::Vector2f> players;

	//sf::UdpSocket* socket;

	void update(float dt);

	Server();
	~Server();
};

