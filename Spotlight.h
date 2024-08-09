#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <vector>

#include "Light.h"
#include "Camera.h"

class Spotlight : public Light
{
public:
	glm::vec3 position;
	glm::vec3 direction;

	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;

	Spotlight();

	//static void setShaderVariables(Shader& shader, std::vector<Spotlight> spotLights);
	static void setShaderVariables(Shader& shader, Camera& camera, bool flashlightIsEnabled);
};

#endif