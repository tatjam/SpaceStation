#pragma once

#include <SFML/Graphics.hpp>


#include "AssetManager.h"

#define TILE_SIZE 32

struct Tile
{
	std::string id = "";

	float topHealth = 0.0f;
	float botHealth = 0.0f;

	sf::Color light = sf::Color::White;

	// TODO: ENTITY LIST, ETC...

};

class Tilemap
{

private:

	sf::Sprite sInstance;

public:

	AssetManager* assets;

	std::vector<Tile> tiles;

	int width, height;

	Tile* get(int x, int y);
	inline int getIndex(int x, int y);

	
	// We use an instanced sf::Sprite, could be improved
	// or even look into shader tilemaps
	// We don't yet use a texture atlas, it may become required
	// if perfomance is bad
	void render(sf::RenderWindow* win, 
		sf::Vector2f offset = sf::Vector2f(0, 0), 
		sf::Vector2f scale = sf::Vector2f(1, 1), float angle = 0.0f);

	void create(int xsize, int ysize);

	void updateServer();
	void updateClient();

	Tilemap(int xsize, int ysize, AssetManager* assetManager);
	~Tilemap();
};

