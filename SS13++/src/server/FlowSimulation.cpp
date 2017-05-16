#include "FlowSimulation.h"

void FlowSimulation::create(int w, int h)
{
	width = w;
	height = h;

	map = std::vector<double>(w * h, 0.0f);
}

void FlowSimulation::draw()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (map[y * width + x] >= 1.0f)
			{
				printf("%i ", (int)(map[y * width + x]));
			}
			else if (map[y * width + x] >= 0.1f)
			{
				printf("; ");
			}
			else if (map[y * width + x] >= 0.001f)
			{
				printf(". ");
			}
			else if(map[y * width + x] > -0.5f)
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

void FlowSimulation::iterate()
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
					if (map[(y + oy) * width + (x + ox)] < -0.5f)
					{
						wallTiles++;
						continue;
					}
				}
			}

			// Simply distribute us in surrounding non-wall tiles
			double give = hold[y * width + x] / (9 - wallTiles);
			give += give * 0.095f;
			

			for (int ox = -1; ox <= 1; ox++)
			{
				for (int oy = -1; oy <= 1; oy++)
				{

					if (y + oy < 0 || x + ox < 0 || y + oy >= height || x + ox >= width)
					{
						continue;
					}
					if (map[(y + oy) * width + (x + ox)] < -0.5f)
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

	printf("Start: %f, End: %f, Delta: %f\n", totalEnergyStart, totalEnergyEnd, -(totalEnergyStart - totalEnergyEnd));

	prevDelta = -(totalEnergyStart - totalEnergyEnd);
}


FlowSimulation::FlowSimulation()
{
}


FlowSimulation::~FlowSimulation()
{
}
