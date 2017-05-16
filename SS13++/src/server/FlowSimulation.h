#pragma once

#include <iostream>
#include <vector>

struct FlowPoint
{
	float val = 0.0f;
};

class FlowSimulation
{
public:

	int width, height;

	std::vector<double> map;

	double prevDelta = 0.0f;

	void iterate();

	void draw();

	void create(int width, int height);

	FlowSimulation();
	~FlowSimulation();
};

