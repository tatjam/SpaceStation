#include "FlowSimulation.h"

void FlowSimulation::create(int w, int h)
{
	width = w;
	height = h;

	map = std::vector<double>(w * h, 0.0f);
}

void FlowSimulation::draw(sf::RenderWindow* win, float max, sf::Vector2f offset,
	sf::Vector2f tileSize, sf::Color color, sf::Color wallColor)
{

	sf::RectangleShape rect;
	rect.setSize(tileSize);


	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{


			float rx = x * tileSize.x + offset.x;
			float ry = y * tileSize.y + offset.y;

			if (map[y * width + x] < -0.4)
			{
				// Wall
				rect.setFillColor(wallColor);

				rect.setPosition(sf::Vector2f(rx, ry));

				win->draw(rect);

				continue;
			}

			float intensity = map[y * width + x] / max; 
			if (intensity > 1.0f)
			{
				intensity = 1.0f;
			}

			rect.setFillColor(sf::Color(
				(float)color.r * intensity, (float)color.g * intensity, (float)color.b * intensity));

			rect.setPosition(sf::Vector2f(rx, ry));


			win->draw(rect);
		}
	}
}

void FlowSimulation::termDraw()
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

void FlowSimulation::iterateFast()
{
	// Flood fill system

}

void FlowSimulation::iterateCell()
{
	double totalEnergyStart = 0.0f;
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
	std::vector<double> hold = map;
	map = std::vector<double>(width * height, 0.0f);



	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (hold[y * width + x] < -0.5)
			{
				map[y * width + x] = hold[y * width + x];
				continue;
			}

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
			double give = hold[y * width + x] / (double)(9 - wallTiles);

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
	}

	double totalEnergyEnd = 0.0f;
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

	//printf("Start: %f, End: %f, Delta: %f\n", totalEnergyStart, totalEnergyEnd, -(totalEnergyStart - totalEnergyEnd));

	prevDelta = -(totalEnergyStart - totalEnergyEnd);
}


FlowSimulation::FlowSimulation()
{
}


FlowSimulation::~FlowSimulation()
{
}
