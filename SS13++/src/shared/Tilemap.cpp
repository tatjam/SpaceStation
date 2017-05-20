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
	sf::Vector2f offset, sf::Vector2f size)
{

	sInstance.setScale(size);

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			float rx = x * TILE_SIZE * size.x + offset.x;
			float ry = y * TILE_SIZE * size.y + offset.y;

			int i = y * width + x;


			sInstance.setPosition(sf::Vector2f(rx, ry));
			sInstance.setColor(tiles[i].light);

			if (tiles[i].bot != NULL && tiles[i].botHealth > 0.0f)
			{
				sInstance.setTexture(*tiles[i].bot->tex);
				win->draw(sInstance);
			}

			if (tiles[i].top != NULL && tiles[i].topHealth > 0.0f)
			{
				sInstance.setTexture(*tiles[i].top->tex);
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

void Tilemap::resize(int nsizex, int nsizey)
{
	tiles.resize(nsizex * nsizey, Tile());
	width = nsizex;
	height = nsizey;
}

// Tilemap serialized file structure
// $$DIC$$\n
// tilename1\n
// tilename2\n
// tilename3\n
// tilename4\n
// $$DEF$$\n
// width,height,creator,name,description
// $$DAT$$\n
// 1.2,3.4,2.1,2.4,2.1,2.1,2.3,2.1,2.3,2.1,1.2,33.23\n  // bot.top,bot.top,...
// $$END$$\n

void Tilemap::deserialize(std::string in)
{
	// First get dictionary
	std::map<int, TileInfo*> dic;
	
	int i = 0;
	// Traverse to start of dictionary
	for (i = 0; i < in.size(); i++)
	{
		if (in[i] == '\n')
		{
			break;
		}
	}

	i++;

	// Read dictionary

	std::string buff = "";

	int ti = 0;

	for (i; i < in.size(); i++)
	{
		if (in[i] == '$')
		{
			break;
		}
		if (in[i] == '\n')
		{
			if (buff == "null")
			{
				dic[ti] = NULL;
			}
			else
			{
				dic[ti] = assets->getTile(buff);
				printf("Read tile: '%s'\n", buff.c_str());
				if (dic[ti] == NULL)
				{
					printf("Invalid tile '%s' read\n", buff.c_str());
				}
			}
			ti++;
			buff = "";
			i++;
		}
		buff += in[i];
	}

	// Now read definition stuff
	for (i; i < in.size(); i++)
	{
		if (in[i] == '\n')
		{
			break;
		}
	}

	i++;

	int nw, nh;
	std::string creator, name, desc;

	buff = "";
	int pt = 0;
	for (i; i < in.size(); i++)
	{
		if (in[i] == '\n')
		{
			break;
		}

		if (in[i] == ',')
		{
			if (pt == 0)
			{
				nw = std::stoi(buff);
			}
			else if (pt == 1)
			{
				nh = std::stoi(buff);
			}
			else if (pt == 2)
			{
				creator = buff;
			}
			else if (pt == 3)
			{
				name = buff;
			}
			else if (pt == 4)
			{
				desc = buff;
			}
			buff = "";
			pt++;
			i++;
		}

		buff += in[i];
	}

	printf("Read definition: (%i, %i). %s (%s) by %s\n", nw, nh, name.c_str(), desc.c_str(), creator.c_str());

	tiles = std::vector<Tile>();

	tiles.resize(nw * nh);
	width = nw;
	height = nh;

	// Read tiles
	for (i; i < in.size(); i++)
	{
		if (in[i] == '\n')
		{
			break;
		}
	}
	i++;

	int tP = 0;
	buff = "";

	std::string buff2;

	bool readingBot = true;

	for (i; i < in.size(); i++)
	{
		if (in[i] == '.')
		{
			readingBot = false;
			continue;
		}
		else if (in[i] == ',')
		{
			// Push tile
			int a = std::stoi(buff);
			int b = std::stoi(buff2);

			tiles[tP].bot = dic[a];
			tiles[tP].top = dic[b];

			buff = "";
			buff2 = "";
			tP++;
			readingBot = true;
			continue;
		}
		else if (in[i] == '\n')
		{
			break;
		}
		
		if(readingBot)
			buff += in[i];
		if (!readingBot)
			buff2 += in[i];

	}
}

std::string Tilemap::serialize()
{
	std::string out = "";

	out += "$$DIC$$\n";
	std::unordered_map<TileInfo*, int> tilesMap;
	std::vector<TileInfo*> tilesv;
	int in = 1;

	tilesMap[NULL] = 0;
	tilesv.push_back(NULL);

	// Add all our tiles from the AssetManager in order
	for (std::unordered_map<std::string, TileInfo>::iterator it = assets->tiles.begin(); it != assets->tiles.end(); ++it)
	{
		tilesv.push_back(&it->second);
		tilesMap[&it->second] = in;
		in++;
	}

	// Write the dictionary
	for (int i = 0; i < tilesv.size(); i++)
	{
		if (tilesv[i] != NULL)
		{
			out += tilesv[i]->cname;
		}
		else
		{
			out += "null";
		}
		out += '\n';
	}
	out += "$$DEF$$\n";
	// Write definition
	out += std::to_string(width);
	out += ",";
	out += std::to_string(height);
	out += ",";
	out += "TODO_CREATOR";
	out += ",";
	out += "TODO_NAME";
	out += ",";
	out += "TODO_DESCRIPTION";
	out += ",\n";
	out += "$$DAT$$\n";
	//Write tile data
	for (int i = 0; i < tiles.size(); i++)
	{
		out += std::to_string(tilesMap[tiles[i].bot]);
		out += ".";
		out += std::to_string(tilesMap[tiles[i].top]);
		out += ",";
	}

	out += "\n";
	out += "$$END$$\n";

	printf("Serialized: \n%s\n", out.c_str());

	return out;
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
