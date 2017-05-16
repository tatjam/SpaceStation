#include "Tilemap.h"



Tile* Tilemap::get(int x, int y)
{
	if (x > 0 && y > 0 && x < width && y < width)
	{
		return &tiles[y * width + x];
	}
	return NULL;
}

inline int Tilemap::getIndex(int x, int y)
{
	return y * width + x;
}

void Tilemap::render(sf::RenderWindow* win,
	sf::Vector2f offset, sf::Vector2f size, float angle)
{

	sInstance.setScale(size);

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			float rx = x * TILE_SIZE * size.x + offset.x;
			float ry = y * TILE_SIZE * size.y + offset.y;

			int i = y * width + x;

			if (tiles[i].id != "")
			{
				// Render tile!

				sInstance.setTexture(*assets->getTex("test.png", false));
				sInstance.setPosition(sf::Vector2f(rx, ry));
				sInstance.setColor(tiles[i].light);
				win->draw(sInstance);
			}
		}
	}
}

void Tilemap::create(int xsize, int ysize)
{
	tiles = std::vector<Tile>();
	tiles.resize(xsize * ysize, Tile());
	width = xsize;
	height = ysize;
}

Tilemap::Tilemap(int xsize, int ysize, AssetManager* assetManager)
{
	create(xsize, ysize);
	sInstance = sf::Sprite();
	assets = assetManager;
}


Tilemap::~Tilemap()
{
}
