//
//  vertex.cpp
//  lattice
//
//  Created by Jarred Parr on 2/2/21.
//

#include "vertex.hpp"
#include <iostream>

// Vertex::Vertex(int k, int m, int x) {
//    point_mass = std::make_unique<PointMass>(
//        k /* The Spring Constant */,
//        m /* THe Mass of the Vertex */,
//        x /* The Initial Position of the Vertex */
//    );
//}

void Vertex::Render() {
//    GLfloat vertices[] = {
//        // Positions        // Colors
//        -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // Bottom Right
//        -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
//        -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f,  // Top
//        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // Bottom Right
//        -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
//        -1.0f,  1.0f,  -1.0f, 1.0f, 0.0f, 0.0f  // Top
//    };
    
    vertices = std::array<GLfloat, 18>{{
        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Right
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
        0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f,  // Top
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), static_cast<void*>(vertices.data()), GL_STATIC_DRAW);

    // Configure Position Attribute
    glVertexAttribPointer(
        0 /* One Vertex Buffer, first attribute location */,
        3 /* Using vec3 to represent the positions of vertices */,
        GL_FLOAT /* Buffer type is float */,
        GL_FALSE, 6 * sizeof(GL_FLOAT),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Configure Color Attribute
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
