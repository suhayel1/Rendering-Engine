#include "Shape.h"

Shape::Shape(glm::vec3 position, Material material, ModelMatrix matrix, int selectedMaterial, int diffuseMap, int specularMap) {
	this->position = position;
	this->material = material;
	this->matrix = matrix;
	this->selectedMaterial = selectedMaterial;
	this->diffuseMap = diffuseMap;
	this->specularMap = specularMap;
}

//Shape::~Shape() {}

void Shape::setup() {}

void Shape::draw() {}
