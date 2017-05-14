#include "RSocket.h"

void RSocket::bind(unsigned short port)
{
	socket->bind(port);
}

void RSocket::send(NetCommand cmd, sf::IpAddress target, unsigned short port)
{
	RMessage n = RMessage();
	n.addr = target;
	n.port = port;
	n.command = cmd;
	n.time = 0.0f;
	sent.push_back(n);
}

RMessage* RSocket::update(float dt)
{
	char receive[PACKET_SIZE];
	std::size_t receiveSize;

	sf::IpAddress ip;
	unsigned short port;

	if (socket->receive(receive, PACKET_SIZE, receiveSize, ip, port))
	{
		NetCommand cmd = NetUtil::breakMessage(receive, receiveSize);
		if (cmd.command == "REPLY")
		{
			for (int i = 0; i < sent.size(); i++)
			{
				if (sent[i].command.id == std::stoll(cmd.args[0])) 
				{
					sent.erase(sent.begin() + i);
				}
			}
		}
		else
		{
			RMessage n = RMessage();
			n.addr = ip;
			n.port = port;
			n.time = -1.0f;
			n.command = cmd;
			received = n;

			// Send that we received the message
			NetCommand cmd;
			cmd.command = "REPLY";
			cmd.args.push_back(std::to_string(cmd.id));

			return &received;
		}
	}

	for (int i = 0; i < sent.size(); i++)
	{
		sent[i].time += dt;
		if (sent[i].time > RSOCKET_INTERVAL)
		{
			// Resend
			sent[i].time = 0.0f;
			NetUtil::sendMessage(socket, sent[i].command, sent[i].addr, sent[i].port);
		}
	}

	return NULL;
}

RSocket::RSocket()
{
	socket = new sf::UdpSocket();
}


RSocket::~RSocket()
{
}
