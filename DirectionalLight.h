//#pragma once
#ifndef DIR_LIGHT_H
#define DIR_LIGHT_H

#include <vector>

#include "Light.h"

class DirectionalLight : public Light
{
public:
	glm::vec3 direction;

	DirectionalLight();

	static void setShaderVariables(Shader& shader, std::vector<DirectionalLight> dirLights);
};

#endif