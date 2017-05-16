#include "Tilemap.h"



inline int Tilemap::getIndex(int x, int y)
{
	return y * width + x;
}

void Tilemap::render(sf::RenderWindow* win,
	sf::Vector2f offset, sf::Vector2f size, float angle)
{
	
}

void Tilemap::create(int xsize, int ysize)
{
	tiles = std::vector<Tile>();
	tiles.resize(xsize * ysize, Tile());
	width = xsize;
	height = ysize;
}

Tilemap::Tilemap(int xsize, int ysize)
{
	create(xsize, ysize);
	sInstance = sf::Sprite();
}


Tilemap::~Tilemap()
{
}
