#pragma once
#include "../Entity.h"
#include <SFML/Graphics.hpp>

#include "../Tilemap.h"

class Tilemap;

// A dynamic entity is an entity that can:
//		- Exist in a location of a tilemap
//		- Change location in real time
//		- Receive net commands
// The base DynamicEntity does nothing, but is not abstract
// WHEN A DYNAMIC ENTITY MOVES IT MUST SET ITSELF
// AS A CHILD OF A TILE IN THE TILEMAP!

class DynamicEntity : public Entity
{
public:

	// Position in tilemap coordinates!
	sf::Vector2i pos = sf::Vector2i(0, 0);

	virtual void update(float dt, Tilemap* tilemap) { }
	virtual void draw() { }
	// TODO
	virtual void command(std::string command, Tilemap* tilemap) {}

	virtual sf::Vector2i* getPos() { return &pos; }

	DynamicEntity() {}
	~DynamicEntity() {}
};

