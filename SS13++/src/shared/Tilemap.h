#pragma once

#include <SFML/Graphics.hpp>

struct Tile
{
	sf::Color light = sf::Color::Black;
	float health = 100.0f;
	std::string id = "air";

	Tile()
	{
		light = sf::Color::Black;
		health = 100.0f;
		id = "air";
	}
};

class Tilemap
{
public:

	std::vector<Tile> tiles;

	int width;

	Tile* get(int x, int y);
	inline int getIndex(int x, int y);

	void render(sf::RenderWindow* win, sf::Vector2f offset);

	void create(int xsize, int ysize);

	void updateServer();
	void updateClient();

	Tilemap(int xsize, int ysize);
	~Tilemap();
};

