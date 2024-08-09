#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

#include "Shader.h"

class Material
{
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

public:
	Material(glm::vec3 ambient = glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f), float shininess = 0.5);
	Material(const Material& material);

	glm::vec3 getAmbientIntensity();
	glm::vec3 getDiffuseColor();
	glm::vec3 getSpecularColor();
	float getShininess();

	void setAmbientIntensity(float ambRed, float ambGreen, float ambBlue);
	void setDiffuseColor(float diffRed, float diffGreen, float diffBlue);
	void setSpecularColor(float specRed, float specGreen, float specBlue);
	void setShininess(float shininess);
	void setShaderVariables(Shader& shader);
};

#endif