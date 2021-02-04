//
//  vertex.cpp
//  lattice
//
//  Created by Jarred Parr on 2/2/21.
//

#include "vertex.hpp"
#include <iostream>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

void Transform(const glm::vec3& transform) {
    
}

void Vertex::Render() {
    v1 = glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f);
    v2 = glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
    v3 = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    
    auto trans = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
    
    v1 = trans * v1;
    v2 = trans * v2;
    v3 = trans * v3;
    
    vertices = std::vector<GLfloat>{{
        v1.x, v1.y, v1.z, RED[0], RED[1], RED[2], // Bottom Right
        v2.x, v2.y, v2.z, RED[0], RED[1], RED[2], // Bottom Left
        v3.x, v3.y, v3.z, RED[0], RED[1], RED[2] // Top
    }};
    
    // Load the being-used vertex objects into memory for later use
    glGenVertexArrays(1, &vertex_array_object);
    
    // Initialize our class-owned vertex buffer on the GPU
    glGenBuffers(1, &vertex_buffer_object);

    // Bind the Vertex Array Object to track the vertices
    glBindVertexArray(vertex_array_object);

    // Bind Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

    // Bind Vertices and Attribute Pointers and Reserve the Memory in the GPU
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), static_cast<void*>(vertices.data()), GL_STATIC_DRAW);

    // Configure Position Attribute, pass up to the shader
    glVertexAttribPointer(
        0 /* One Vertex Buffer, first attribute location */,
        3 /* Using vec3 to represent the positions of vertices */,
        GL_FLOAT /* Buffer type is float */,
        GL_FALSE, 6 * sizeof(GL_FLOAT),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Configure Color Attribute, pass up to the shader
    glVertexAttribPointer(
        1 /* Second Attribute Location */,
        3 /* Using a vec3 to represent the positions */,
        GL_FLOAT /* Buffer type is float */,
        GL_FALSE /* Do not normalize */,
        6 * sizeof(GL_FLOAT) /* Offset between data points */,
                          (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Unbind Vertex Array Object
    glBindVertexArray(0);

    is_init = true;
}
