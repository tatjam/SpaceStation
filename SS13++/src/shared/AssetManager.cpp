#include "AssetManager.h" 

using namespace nlohmann;

void AssetManager::loadTiles(std::string path, bool ignoreRoot)
{
	std::ifstream file(path);

	if (!file.bad())
	{
		json j;
		file >> j;

		std::string baseDirt, baseDamage;

		baseDirt = j["base_dirt"].get<std::string>();
		baseDamage = j["base_damage"].get<std::string>();


		for (json::iterator it = j.begin(); it != j.end(); ++it)
		{
			json sub = j[it.key()];
			if (sub.is_object())
			{

				// Load a tile
				TileInfo in;
				in.top = getTex(sub["top"]);
				in.bot = getTex(sub["bot"]);
				if (sub["dirt"].is_null())
				{
					in.dirt = getTex(baseDirt);
				}
				else
				{
					in.dirt = getTex(sub["dirt"]);
				}
				if (sub["dirt"].is_null())
				{
					in.damage = getTex(baseDamage);
				}
				else
				{
					in.damage = getTex(sub["damage"]);
				}

				in.walkable = (bool)sub["walkable"];
				in.transparent = (bool)sub["transparent"];

				in.topHealth = sub["tophealth"];
				in.botHealth = sub["bothealth"];

				tiles[it.key()] = in;

			}
		}


	}
	else
	{
		printf("Could not load tile .json: %s\n", path.c_str());
	}
}

TileInfo* AssetManager::getTile(std::string name)
{
	if (tiles.count(name))
	{
		return &tiles[name];
	}

	return NULL;
}

sf::Texture* AssetManager::getTex(std::string path, bool ignoreRoot)
{
	if (textures.count(path))
	{
		return &textures[path];
	}
	else
	{
		return loadTex(path, ignoreRoot);
	}

	return NULL;
}

sf::Texture* AssetManager::loadTex(std::string path, bool ignoreRoot)
{
	sf::Texture n = sf::Texture();

	std::string full;
	if (!ignoreRoot)
	{
		full = root;
	}
	if (n.loadFromFile(full + path))
	{
		textures[path] = n;
		return &textures[path];
	}

	return NULL;
	
}

AssetManager::AssetManager()
{
}

AssetManager::AssetManager(std::string resRoot)
{
	root = resRoot;
}


AssetManager::~AssetManager()
{
}
