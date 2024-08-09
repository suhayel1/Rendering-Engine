#include "ModelMatrix.h"

ModelMatrix::ModelMatrix(glm::vec3 scale, glm::vec3 rotation, glm::vec3 translation) {
	this->scale = scale;
	this->rotation = rotation;	// in degrees
	this->translation = translation;
}

glm::mat4 ModelMatrix::getMatrix() {
	// translation matrix
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), this->translation);

	// rotation matrix with quaternion
	glm::vec3 rotationInRadians = glm::vec3(glm::radians(this->rotation[0]), glm::radians(this->rotation[1]), glm::radians(this->rotation[2]));
	glm::quat quaternion = glm::quat(rotationInRadians);
	glm::mat4 rotationMatrix = glm::toMat4(quaternion);

	// scale matrix (uniform scaling for now)
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), this->scale);

	// update model matrix
	return translationMatrix * rotationMatrix * scaleMatrix;
}