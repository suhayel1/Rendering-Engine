#include "Material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) {
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
}

Material::Material(const Material& material) {
	this->ambient = material.ambient;
	this->diffuse = material.diffuse;
	this->specular = material.specular;
	this->shininess = material.shininess;
};

glm::vec3 Material::getAmbientIntensity() {
	return this->ambient;
}

glm::vec3 Material::getDiffuseColor() {
	return this->diffuse;
}

glm::vec3 Material::getSpecularColor() {
	return this->specular;
}

float Material::getShininess() {
	return this->shininess * 128.0f;	// stated in http://devernay.free.fr/cours/opengl/materials.html to multiply shininess by 128
}

void Material::setAmbientIntensity(float ambRed, float ambGreen, float ambBlue) {
	this->ambient = glm::vec3(ambRed, ambGreen, ambBlue);
}

void Material::setDiffuseColor(float diffRed, float diffGreen, float diffBlue) {
	this->diffuse = glm::vec3(diffRed, diffGreen, diffBlue);
}

void Material::setSpecularColor(float specRed, float specGreen, float specBlue) {
	this->specular = glm::vec3(specRed, specGreen, specBlue);
}

void Material::setShininess(float shininess) {
	this->shininess = shininess;
}

void Material::setShaderVariables(Shader& shader) {
	shader.setVec3("material.ambient", this->ambient);
	shader.setVec3("material.diffuse", this->diffuse);
	shader.setVec3("material.specular", this->specular); // specular lighting doesn't have full effect on this object's material
	shader.setFloat("material.shininess", this->shininess * 128.0f);
}
