#pragma once

#include <SFML/Network.hpp>

#define PACKET_SIZE 8192
#define ARG_SEPARATOR '|'//'\x1f'
#define ARG_START '$'

// Low level network helpers
// C-like, using malloc & stuff

typedef unsigned long long ncid;

struct NetCommand
{
	ncid id = 0;
	std::string command = "INVALID";
	std::vector<std::string> args;
};


class NetUtil
{
public:

	static NetCommand breakMessage(char* packet, std::size_t size);
	static char* formMessage(NetCommand command, std::size_t* outsize);
	static sf::Socket::Status sendMessage(sf::UdpSocket* socket, NetCommand command, sf::IpAddress ip, unsigned short port);
	static std::string toString(NetCommand command);
};