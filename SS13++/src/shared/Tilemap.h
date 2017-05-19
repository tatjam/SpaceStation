#pragma once

#include <SFML/Graphics.hpp>


#include "AssetManager.h"

#include "ECS/DynamicEntity.h"

#define TILE_SIZE 32


//          TILES
// 
// Tiles are very basic blocks.
// They have no interaction and just build up a physical
// station. They have a top and bottom layer


struct Tile
{
	std::string id = "";

	// Top health is what blocks the 
	// player from moving through
	// Leaks only if next to empty tiles
	float topHealth = 0.0f;

	// Bot health is the "ground" of the 
	// tile, if it breaks there is a leak
	float botHealth = 0.0f;

	sf::Color light = sf::Color::White;
	
	// Entities on top of this tile!
	// When an entity moves it's his responsability
	// to get put in the correct tile!
	std::vector<DynamicEntity*> entities;

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
		sf::Vector2f scale = sf::Vector2f(1, 1));

	void create(int xsize, int ysize);

	void resize(int nsizex, int nsizey);

	// We must check for any MDATA commands
	void updateServer();

	// We download required info from server
	void updateClient();

	Tilemap(int xsize, int ysize, AssetManager* assetManager);
	~Tilemap();
};

