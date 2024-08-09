#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"
#include "Cube.h"

class PointLight : public Light
{
public:
	Cube shape;

	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;

	PointLight();

	static void setShaderVariables(Shader& shader, std::vector<PointLight> pointLights);
};

#endif