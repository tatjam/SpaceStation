#pragma once

#include <iostream>
#include <vector>

#include "SFML/Graphics.hpp"


class FlowSimulationOld
{
public:

	int width, height;

	std::vector<double> map;

	double prevDelta = 0.0f;

	void iterateCell();

	void iterateFast();

	void termDraw();

	void draw(sf::RenderWindow* win, float max = 100.0f, sf::Vector2f offset = sf::Vector2f(0, 0),
		sf::Vector2f tileSize = sf::Vector2f(10.0f, 10.0f), 
		sf::Color color = sf::Color::Blue, sf::Color wallColor = sf::Color::Green);

	void create(int width, int height);


	FlowSimulationOld();
	~FlowSimulationOld();
};

