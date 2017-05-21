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

// This can be optimized and made better! Please look into it future you
bool Tilemap::canSee(int x1, int y1, int x2, int y2, int maxDist)
{
	if (maxDist <= 0)
	{
		maxDist = 99999999;
	}

	float dist = std::sqrt(std::powf((float)(x1 - x2), 2.0f) + std::powf((float)(y1 - y2), 2.0f));

	if (dist > maxDist)
	{
		return false;
	}
	
	// Sanitize data for our algorithm
	// P(x1, y1) must be < P(x2, y2)
	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	// Same point
	if (x1 == x2 && y1 == y2)
	{
		return true;
	}

	// Special case for vertical line
	if (x2 == x1)
	{
		if (y1 > y2)
		{
			std::swap(y1, y2);
			std::swap(x1, x2);
		}

		for (int y = y1; y <= y2; y++)
		{
			int i = y * width + x1;

			if (tiles[i].top != NULL)
			{
				if (tiles[i].top->transparent == false)
				{
					return false;
				}
			}
		}

		return true;
	}

	// Get equation for line
	// (y=mx+b)
	float m, b;
	// m = (y2-y1)/(x2-x1)
	m = (float)(y1 - y2) / (float)(x1 - x2);
	// b = -mx1 + y1
	b = -m * (float)x1 + (float)y1;

	/*float x = x1 + 0.5f;
	float y = y1;*/
	int rx = x1;
	int ry = y1;
	
	float x = x1 + 0.5f;
	float y;


	float xstep = 1 / std::abs(m);
	if (xstep > 0.8f)
	{
		xstep = 0.8f;
	}

	for (x; x <= x2; x += xstep)
	{
		float y = m * x + b;
		//y += 0.1f;
		rx = (int)std::roundf(x);
		ry = (int)std::roundf(y);
		if (rx >= 0 && rx < width && ry >= 0 && ry < height)
		{
			int i = ry * width + rx;

			float ay = y - m / 4;

			int rry = (int)std::roundf(ay);

			int i2 = rry * width + rx;

			if(rry >= 0 && rry < width)
			{ 

				if (tiles[i2].top != NULL)
				{
					if (tiles[i2].top->transparent == false)
					{
						return false;
					}
				}
			}

	


			if (tiles[i].top != NULL)
			{
				if (tiles[i].top->transparent == false)
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

// Works but is pretty fucked up :P
void Tilemap::doLight(int x, int y, sf::Color color, int power, float mult)
{
	int minX = x - power;
	int minY = y - power;
	int maxX = x + power;
	int maxY = y + power;
	if (minX < 0) { minX = 0; }
	if (minY < 0) { minY = 0; }
	if (maxX > width) { maxX = width; }
	if (maxY > height) { maxY = height; }


	for (int ox = minX; ox < maxX; ox++)
	{
		for (int oy = minY; oy < maxY; oy++)
		{
			int i = oy * width + ox;

			float dist = std::sqrt(std::powf((float)(ox - x), 2.0f) + std::powf((float)(oy - y), 2.0f));
			float lPow = (power / dist) - 1.0f;

			if(!canSee(ox, oy, x, y, power * 2))
			{ 
				lPow = 0.0f;
			}


			float mR = color.r * lPow * mult; float mG = color.g * lPow * mult; float mB = color.b * lPow * mult;
			float fR = mR + tiles[i].light.r; float fG = mG + tiles[i].light.g; float fB = mB + tiles[i].light.b;
			if (fR > 255.0f) { fR = 255.0f; }
			if (fG > 255.0f) { fG = 255.0f; }
			if (fB > 255.0f) { fB = 255.0f; }
			if (fR < 0.f) { fR = 0.0f; }
			if (fG < 0.f) { fG = 0.0f; }
			if (fB < 0.f) { fB = 0.0f; }

			sf::Color finalColor = sf::Color(fR, fG, fB);

			tiles[i].light = finalColor;

		}
	}

	// Perform wall visibility checks
	for (int ox = minX; ox < maxX; ox++)
	{
		for (int oy = minY; oy < maxY; oy++)
		{
			int i = oy * width + ox;
			if (tiles[i].top != NULL)
			{
				if (tiles[i].top->transparent == false)
				{
					// Light bleed
					for (int oox = -1; oox <= 1; oox++)
					{
						for (int ooy = -1; ooy <= 1; ooy++)
						{
							// Ignore * 
							// (*#*) (-1,-1)(1,-1)
							// (###) 
							// (*#*) (-1, 1)(1, 1)
							// Comment to make lighting non-symmetrical 
							// (or after fixing the visibility checking)
							if ((oox == -1 && ooy == -1) || (oox == 1 && ooy == -1) ||
								(oox == -1 && ooy == 1) || (oox == 1 && ooy == 1))
							{
								continue;
							}
							
							if (ox + oox >= 0 && ox + oox < width && oy + ooy >= 0 && oy + ooy < height)
							{
								int ii = (oy + ooy) * width + (ox + oox);
								if (tiles[ii].top != NULL)
								{
									if (tiles[ii].top->transparent == true)
									{
										if (tiles[ii].light != sf::Color::Black)
										{
											tiles[i].light = tiles[ii].light;
										}
									}

								}
								else
								{
									if (tiles[ii].light != sf::Color::Black)
									{
										tiles[i].light = tiles[ii].light;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	// Wohoo nested blocks!
}

void Tilemap::clearLight(int minX, int minY, int maxX, int maxY, sf::Color color)
{
	for (int ox = minX; ox < maxX; ox++)
	{
		for (int oy = minY; oy < maxY; oy++)
		{
			int i = oy * width + ox;
			tiles[i].light = color;
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
