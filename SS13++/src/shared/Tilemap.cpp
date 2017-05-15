#include "Tilemap.h"



inline int Tilemap::getIndex(int x, int y)
{
	return y * width + x;
}

void Tilemap::create(int xsize, int ysize)
{
	tiles = std::vector<Tile>();
	tiles.resize(xsize * ysize, Tile());
	width = xsize;
}

Tilemap::Tilemap(int xsize, int ysize)
{
	create(xsize, ysize);
}


Tilemap::~Tilemap()
{
}
