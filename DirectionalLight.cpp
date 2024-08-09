#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light() {
	this->direction = glm::vec3();
}

void DirectionalLight::setShaderVariables(Shader& shader, std::vector<DirectionalLight> dirLights) {
    for (int i = 0; i < dirLights.size(); i++) {
        Light light = Light();
        std::string j = std::to_string(i);

        if (dirLights[i].isEnabled) light = Light(&dirLights[i]);
        
        light.setShaderVariables(shader, "dirLights", j);

        shader.setVec3("dirLights[" + j + "].direction", dirLights[i].direction);
        shader.setBool("dirLights[" + j + "].isEnabled", dirLights[i].isEnabled);
    }
    shader.setInt("numDirLights", dirLights.size());
}