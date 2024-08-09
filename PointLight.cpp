#include "PointLight.h"

PointLight::PointLight() : Light(){
	this->shape = Cube();
	this->shape.matrix.scale = glm::vec3(0.1f, 0.1f, 0.1f);

	this->position = glm::vec3();

	this->constant = 0.0f;
	this->linear = 0.0f;
	this->quadratic = 0.0f;
}

void PointLight::setShaderVariables(Shader& shader, std::vector<PointLight> pointLights) {
    for (int i = 0; i < pointLights.size(); i++) {
        Light light = Light();
        std::string j = std::to_string(i);

        if (pointLights[i].isEnabled) light = Light(&pointLights[i]);

        light.setShaderVariables(shader, "pointLights", j);

        shader.setVec3("pointLights[" + j + "].position", pointLights[i].position);
        shader.setFloat("pointLights[" + j + "].constant", 1.0f);
        shader.setFloat("pointLights[" + j + "].linear", 0.09f);
        shader.setFloat("pointLights[" + j + "].quadratic", 0.032f);
        shader.setBool("pointLights[" + j + "].isEnabled", pointLights[i].isEnabled);
    }
    shader.setInt("numPointLights", pointLights.size());
}