#pragma once
#include "NetUtil.h"

#define RSOCKET_INTERVAL 0.3f

struct RMessage
{
	NetCommand command;
	sf::IpAddress addr;
	unsigned short port;

	float time = 0.0f;
};

// A RobustSocket, read the implementation
// Wraps over an UDP socket and implements the
// SS13++ Network Protocal, aka Robust Sockets
class RSocket
{
public:

	ncid id = 0;

	RMessage received;

	std::vector<RMessage> sent;

	sf::UdpSocket* socket;

	// Returns null if no new message, otherwise returns it
	// Will handle the arrival of messages automatically
	RMessage* update(float dt);

	void send(NetCommand cmd, sf::IpAddress target, unsigned short port);
	
	void bind(unsigned short port);

	RSocket();
	~RSocket();
};

