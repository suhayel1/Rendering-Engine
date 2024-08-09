#include "Cube.h"

std::vector<float> Cube::vertices = {
    // positions            // normals
    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f
};

Cube::Cube() : Shape() {
    this->setup();
}

void Cube::setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(), &this->vertices[0], GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Cube::draw() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
