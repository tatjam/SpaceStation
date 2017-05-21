#pragma once

#include <unordered_map>

#include <SFML/Graphics.hpp>

#include <fstream>

#include "../dep/json.h"

struct TileInfo
{
	bool top = false;

	sf::Texture* tex;

	sf::Texture* dirt;

	sf::Texture* damage;

	std::string name;
	std::string cname;
	std::string desc;


	bool transparent;
	bool blockAir;
	bool walkable;

	float health;
};

// All operations are done under "root" directory!
// That means that any resource outside of it is not meant to be accessed
// but can be by giving the ignoreRoot flag!
class AssetManager
{
public:

	std::string root = "res/";

	std::unordered_map<std::string, TileInfo> tiles;

	std::unordered_map<std::string, sf::Texture> textures;

	// Tries to get texture, if it's not available
	// it will be loaded from the resource folder
	sf::Texture* getTex(std::string path, bool ignoreRoot = false);

	// Loads a texture from disk
	// It's not meant to be called manually!
	sf::Texture* loadTex(std::string path, bool ignoreRoot = false);

	void loadTiles(std::string path, bool ignoreRoot = false);

	TileInfo* getTile(std::string name);

	// Load .json file and reads all resources there
	void readResources(std::string path, bool ignoreRoot = false);

	AssetManager();
	AssetManager(std::string resRoot);
	~AssetManager();
};

