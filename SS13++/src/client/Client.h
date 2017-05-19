#pragma once
#include <SFML/Network.hpp>
#include "../server/Server.h"


#include "../dep/imgui/imgui.h"
#include "../dep/imgui/imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Client
{
private:

	sf::Clock cClock;
	ncid id;

public:

	sf::RenderWindow* win;

	bool connected = false;
	bool connecting = false;
	std::string name;

	float timer = 0.0f;

	NetSender server;

	RSocket* socket;

	void update(float dt);

	void connect(NetSender server);

	void display();

	Client();
	~Client();
};

