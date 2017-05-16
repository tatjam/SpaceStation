#include "FlowSimulationOld.h"

void FlowSimulationOld::create(int w, int h)
{
	width = w;
	height = h;

	map = std::vector<float>(w * h, 0.0f);

	pixels = (sf::Uint8*)malloc(width * height * 4 * sizeof(sf::Uint8));
}

void FlowSimulationOld::draw(sf::RenderWindow* win, float max, sf::Vector2f offset,
	sf::Vector2f tileSize, sf::Color color, sf::Color wallColor)
{


	sf::RectangleShape rect;


	for(int i = 0; i < width * height; i++)
	{
		if (map[i] > 0 && map[i] < 0.3)
		{
			continue;
		}
		if (map[i] < -0.4)
		{
			pixels[i * 4 + 0] = wallColor.r;
			pixels[i * 4 + 1] = wallColor.g;
			pixels[i * 4 + 2] = wallColor.b;
			pixels[i * 4 + 3] = wallColor.a;
			continue;
		}
		float in = map[i] / max;
		if (in > 1.0f)
		{
			in = 1.0f;
		}
		pixels[i * 4 + 0] = color.r * in;
		pixels[i * 4 + 1] = color.g * in;
		pixels[i * 4 + 2] = color.b * in;
		pixels[i * 4 + 3] = color.a;
	}

	img.create(width, height, pixels);
	tex.loadFromImage(img);

	rect.setTexture(&tex);
	rect.setSize(sf::Vector2f(width * tileSize.x, height * tileSize.y));
	win->draw(rect);

}

void FlowSimulationOld::termDraw()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (map[y * width + x] >= 0.95)
			{
				printf("%i ", (int)std::round(map[y * width + x]));
			}
			else if (map[y * width + x] > 0.1)
			{
				printf("; ");
			}
			else if (map[y * width + x] >= 0.001)
			{
				printf(". ");
			}
			else if(map[y * width + x] > -0.5)
			{
				printf("  ");
			}
			else
			{
				printf("# ");
			}
		}

		printf("\n");
	}
}

void FlowSimulationOld::iterateFast()
{
	// Flood fill system

}

void FlowSimulationOld::iterateCell()
{
	float totalEnergyStart = 0.0f;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (map[y * width + x] > -0.4)
			{
				totalEnergyStart += map[y * width + x];
			}
		}
	}
	std::vector<float> hold = map;
	map = std::vector<float>(width * height, 0.0f);



	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (hold[y * width + x] < -0.3)
			{
				map[y * width + x] = hold[y * width + x];
				continue;
			}

			if (hold[y * width + x] > 0.1)
			{

				int wallTiles = 0;
				for (int ox = -1; ox <= 1; ox++)
				{
					for (int oy = -1; oy <= 1; oy++)
					{

						if (y + oy < 0 || x + ox < 0 || y + oy >= height || x + ox >= width)
						{
							wallTiles++;
							continue;
						}
						if (hold[(y + oy) * width + (x + ox)] < -0.5f)
						{
							wallTiles++;
							continue;
						}
					}
				}

				// Simply distribute us in surrounding non-wall tiles
				float give = hold[y * width + x] / (float)(9 - wallTiles);

				for (int ox = -1; ox <= 1; ox++)
				{
					for (int oy = -1; oy <= 1; oy++)
					{

						if (y + oy < 0 || x + ox < 0 || y + oy >= height || x + ox >= width)
						{
							continue;
						}
						if (hold[(y + oy) * width + (x + ox)] < -0.5f)
						{
							continue;
						}
						map[(y + oy) * width + (x + ox)] += give;
					}
				}
			}
			else
			{
				map[y * width + x] = hold[y * width + x];
			}
		}
	}

	float totalEnergyEnd = 0.0f;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (map[y * width + x] > -0.4)
			{
				totalEnergyEnd += map[y * width + x];
			}
		}
	}

	printf("Start: %f, End: %f, Delta: %f\n", totalEnergyStart, totalEnergyEnd, -(totalEnergyStart - totalEnergyEnd));

	prevDelta = -(totalEnergyStart - totalEnergyEnd);
}


FlowSimulationOld::FlowSimulationOld()
{

}


FlowSimulationOld::~FlowSimulationOld()
{
}
