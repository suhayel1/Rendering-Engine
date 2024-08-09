//#pragma once
#ifndef CUBE_H
#define CUBE_H

//#include <glm/glm.hpp>

#include "Shape.h"

class Cube : public Shape
{
public:
	static std::vector<float> vertices;

	Cube();

	void setup() override;
	void draw() override;
};

#endif