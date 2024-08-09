#ifndef MODEL_MATRIX_H
#define MODEL_MATRIX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class ModelMatrix
{
public:
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec3 translation;

	ModelMatrix(glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 rotation = glm::vec3(), glm::vec3 translation = glm::vec3());

	glm::mat4 getMatrix();
};

#endif