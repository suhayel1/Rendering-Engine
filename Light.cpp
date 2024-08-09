#include "Light.h"

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool isEnabled) {
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->isEnabled = isEnabled;
}

Light::Light(Light* light) {
	this->ambient = light->ambient;
	this->diffuse = light->diffuse;
	this->specular = light->specular;
	this->isEnabled = light->isEnabled;
}

void Light::setShaderVariables(Shader& shader, std::string lightType, std::string i) {
	shader.setVec3(lightType + "[" + i + "].ambient", this->ambient);
	shader.setVec3(lightType + "[" + i + "].diffuse", this->diffuse);
	shader.setVec3(lightType + "[" + i + "].specular", this->specular);
}