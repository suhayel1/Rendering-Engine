#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>
#include <glad/glad.h> // holds all OpenGL type declarations

#include "Material.h"
#include "ModelMatrix.h"

#include <iostream>
#include <vector>

class Shape
{
public:
	glm::vec3 position;
	Material material;
	ModelMatrix matrix;
	unsigned int VBO, VAO, EBO;
	int selectedMaterial;
	int diffuseMap;
	int specularMap;

	Shape(glm::vec3 position = glm::vec3(), Material material = Material(), ModelMatrix matrix = ModelMatrix(), int selectedMaterial = 0, int diffuseMap = 0, int specularMap = 0);
	//~Shape();

	virtual void setup();
	virtual void draw();
};

#endif