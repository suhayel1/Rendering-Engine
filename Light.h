#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <string>

#include "Shader.h"

class Light
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	bool isEnabled;

	Light(glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 diffuse = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f), bool isEnabled = true);
	Light(Light* light);

	void setShaderVariables(Shader& shader, std::string lightType, std::string i);
};

#endif