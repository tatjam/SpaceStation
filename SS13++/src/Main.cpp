#include <SFML/Graphics.hpp>

#include "server/Server.h"
#include "client/Client.h"


#include "shared/SquirrelScript.h"

SQInteger testfun(HSQUIRRELVM vm)
{
	printf("We got called from the VM!\n");
	SQInteger argCount = sq_gettop(vm);
	for (int i = 2; i <= argCount; i++)
	{
		printf("Arg (%i): Type: ", i);
		switch (sq_gettype(vm, i))
		{
		case OT_NULL:
			printf("null");
			break;
		case OT_INTEGER:
			printf("integer");
			break;
		case OT_FLOAT:
			printf("float");
			break;
		case OT_STRING:
			printf("string");
			break;
		case OT_TABLE:
			printf("table");
			break;
		case OT_ARRAY:
			printf("array");
			break;
		case OT_USERDATA:
			printf("userdata");
			break;
		case OT_CLOSURE:
			printf("closure(function)");
			break;
		case OT_NATIVECLOSURE:
			printf("native closure(C function)");
			break;
		case OT_GENERATOR:
			printf("generator");
			break;
		case OT_USERPOINTER:
			printf("userpointer");
			break;
		case OT_CLASS:
			printf("class");
			break;
		case OT_INSTANCE:
			printf("instance");
			break;
		case OT_WEAKREF:
			printf("weak reference");
			break;
		default:
			return sq_throwerror(vm, "invalid param"); //throws an exception
		}
		printf("\n");
	}

	argCount -= 1;

	sq_pushinteger(vm, argCount);
	return 1;
}

int main()
{

	sqScript script = sqScript();
	script.regFunction(testfun, "testfun");
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