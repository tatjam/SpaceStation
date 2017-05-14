#include "NetUtil.h"

std::string NetUtil::toString(NetCommand command)
{
	std::string out = command.command;
	
	out += '[';
	out += std::to_string(command.id);
	out += "](";

	for (int i = 0; i < command.args.size(); i++)
	{
		out.append(command.args[i]);
		if (command.args.size() - i > 1)
		{
			out += ", ";
		}
	}

	out += ")";

	return out;
}

NetCommand NetUtil::breakMessage(char* packet, std::size_t size)
{
	NetCommand out;

	if (packet[0] != ARG_START)
	{
		return out;
	}



	std::vector<std::string> read;

	int pt = 0;

	std::string buff = "";

	int i = 1;

	// Reard as ARG_SEPARATOR separated list
	for (i; i < size; i++)
	{
		if (packet[i] == ARG_SEPARATOR)
		{
			buff += '\0';
			read.push_back(buff);
			buff = "";
			i++;
		}

		if (packet[i] == '\0')
		{
			break;
		}

		buff += packet[i];
	}

	read.push_back(buff);
	buff = "";

	out.command = read[0];
	for (i = 1; i < read.size() - 1; i++)
	{
		out.args.push_back(read[i]);
	}

	out.id = std::stoull(read[read.size() - 1]);

	return out;

}

sf::Socket::Status NetUtil::sendMessage(sf::UdpSocket* socket, NetCommand command, sf::IpAddress address, unsigned short port)
{
	std::size_t size;
	char* cmd = formMessage(command, &size);
	sf::Socket::Status ret = socket->send(cmd, size, address, port);
	free(cmd);
	return ret;
}

char* NetUtil::formMessage(NetCommand command, std::size_t* outsize)
{

	std::string n = std::to_string(command.id);

	std::size_t size = command.command.size() + 2;
	for(int i = 0; i < command.args.size(); i++)
	{
		size += command.args[i].size();
		size += 1; // Separator
	}

	if (size > PACKET_SIZE)
	{
		size = PACKET_SIZE;
	}

	size += n.size();

	size += 2;

	*outsize = size;

	char* out = (char*)malloc(sizeof(char) * size);

	out[0] = ARG_START;



	int argPointer = 0;
	int i = 1;
	// Write command
	for (i; i < size; i++)
	{
		if (i - 1 >= command.command.size())
		{
			break;
		}

		out[i] = command.command[i - 1];
	}

	out[i] = ARG_SEPARATOR;
	i++;

	


	for (int o = 0; o < command.args.size(); o++)
	{
		for (int off = 0; off < command.args[o].size(); off++)
		{
			if (i >= size)
			{
				break;
			}
			out[i] = command.args[o][off];
			i++;
		}
		out[i] = ARG_SEPARATOR;
		i++;

	}

	for (int o = 0; o < n.size(); o++)
	{
		out[i] = n[o];
		i++;
	}

	out[i] = '\0';

	return out;
}