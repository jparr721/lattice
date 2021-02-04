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

// This class is mis-named for now...
Vertex::Vertex() {
    // Hard-Coded Triangle
    const auto v1 = glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f);
    const auto v2 = glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
    const auto v3 = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);

    shape = std::vector<GLfloat>{{
        -1.0f, /* Start First Vertex */
        -1.0f,
        0.0f, /* End First Vertex */
        1.0f, /* Start First Color */
        0.0f,
        0.0f, /* End First Color */
        1.0f, /* Start Second Vertex */
        -1.0f,
        0.0f, /* End Second Vertex */
        1.0f, /* Start Second Color */
        0.0f,
        0.0f, /* End Second Color */
        0.0f, /* Start Third Vertex */
        1.0f,
        0.0f, /* End Third Vertex*/
        1.0f, /* Start Third Color */
        0.0f,
        0.0f, /* End Third Color */
    }};
}

void Vertex::RebuildShape() {
    for (int i = 0; i < vertices.size(); ++i) {
        auto vertex = vertices[i];

        // Stride size is 6
        int j = i * 6;

        shape[j] = vertex.x;
        shape[j + 1] = vertex.y;
        shape[j + 2] = vertex.z;
    }
}

void Vertex::Transform(const glm::vec3& transform) {
    auto transformation_matrix = glm::translate(glm::mat4(1.0f), transform);

    for (auto vertex : vertices) {
        vertex = transformation_matrix * vertex;
    }

    RebuildShape();
}

void Vertex::Render() {
    RebuildShape();

    // Load the being-used vertex objects into memory for later use
    glGenVertexArrays(1, &vertex_array_object);

    // Initialize our class-owned vertex buffer on the GPU
    glGenBuffers(1, &vertex_buffer_object);

    // Bind the Vertex Array Object to track the vertices
    glBindVertexArray(vertex_array_object);

    // Bind Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

    // Bind Vertices and Attribute Pointers and Reserve the Memory in the GPU
    glBufferData(GL_ARRAY_BUFFER, shape.size() * sizeof(float),
                 static_cast<void*>(shape.data()), GL_STATIC_DRAW);

    // Configure Position Attribute, pass up to the shader
    glVertexAttribPointer(
        0 /* One Vertex Buffer, first attribute location */,
        3 /* Using vec3 to represent the positions of vertices */,
        GL_FLOAT /* Buffer type is float */, GL_FALSE, 6 * sizeof(GL_FLOAT),
        (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Configure Color Attribute, pass up to the shader
    glVertexAttribPointer(1 /* Second Attribute Location */,
                          3 /* Using a vec3 to represent the positions */,
                          GL_FLOAT /* Buffer type is float */,
                          GL_FALSE /* Do not normalize */,
                          6 * sizeof(GL_FLOAT) /* Offset between data points */,
                          (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Unbind Vertex Array Object
    glBindVertexArray(0);
}
