#include <SFML/Graphics.hpp>

#include <stdarg.h>
#include "server/Server.h"
#include "client/Client.h"

extern "C"
{

#include "dep/squirrel/include/squirrel.h"
#include "dep/squirrel/include/sqstdio.h"
#include "dep/squirrel/include/sqstdmath.h"
#include "dep/squirrel/include/sqstdstring.h"
#include "dep/squirrel/include/sqstdaux.h"
}


#ifdef SQUNICODE

#define scvprintf vfwprintf
#else

#define scvprintf vfprintf
#endif

#include "dep/squirrel/wrap/SquirrelScript.h"

int main()
{

	sqScript script = sqScript();
	script.load("res/test.nut");
	sqArgument a, b, c;
	a.type = OT_INTEGER; a.iVal = 100;
	b.type = OT_FLOAT; b.fVal = 2.7f;
	c.type = OT_STRING; c.sVal = std::string("Hello World!");
	sqArgList args; 
	args.args.push_back(a); args.args.push_back(b); args.args.push_back(c);
	script.call("foo", args);
	script.call("asd", sqArgList());

	getchar();

	return 0;

	NetCommand cmd;
	cmd.command = "SAY";
	cmd.args.push_back("Well, that was interesting lol!");
	std::size_t outsize;

	NetUtil::formMessage(cmd, &outsize);


	Server server = Server();
	Client client = Client();

	NetSender serverNet;
	serverNet.ip = sf::IpAddress::getLocalAddress();
	serverNet.port = server.socket->socket->getLocalPort();

	sf::Clock dtc = sf::Clock();
	float dt = 0.0f;

	while (true)
	{
		if (client.connected == false)
		{
			client.connect(serverNet);
		}
		server.update(dt);
		client.update(dt);

		client.display();

		dt = dtc.restart().asSeconds();
	}

	return 0;
}