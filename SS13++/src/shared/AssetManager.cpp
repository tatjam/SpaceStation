#include "AssetManager.h" 



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
