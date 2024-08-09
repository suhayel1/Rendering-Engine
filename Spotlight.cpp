#include "Spotlight.h"

Spotlight::Spotlight() : Light(){
	this->position = glm::vec3();
	this->direction = glm::vec3();

	this->constant = 0.0f;
	this->linear = 0.0f;
	this->quadratic = 0.0f;
	this->cutOff = 0.0f;		// in degrees
	this->outerCutOff = 0.0f;	// in degrees
}

//void Spotlight::setShaderVariables(Shader& shader, std::vector<Spotlight> spotLights) {
void Spotlight::setShaderVariables(Shader& shader, Camera& camera, bool flashlightIsEnabled) {
    shader.setVec3("spotLights[0].position", camera.Position);
    shader.setVec3("spotLights[0].direction", camera.Front);
    shader.setVec3("spotLights[0].ambient", 0.0f, 0.0f, 0.0f);
    shader.setVec3("spotLights[0].diffuse", 1.0f, 1.0f, 1.0f);
    shader.setVec3("spotLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("spotLights[0].constant", 1.0f);
    shader.setFloat("spotLights[0].linear", 0.09f);
    shader.setFloat("spotLights[0].quadratic", 0.032f);
    shader.setFloat("spotLights[0].cutOff", glm::cos(glm::radians(12.5f)));
    shader.setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(15.0f)));
    shader.setBool("spotLights[0].isEnabled", flashlightIsEnabled);
}