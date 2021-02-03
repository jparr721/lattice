//
//  vertex.cpp
//  lattice
//
//  Created by Jarred Parr on 2/2/21.
//

#include "vertex.hpp"

// Vertex::Vertex(int k, int m, int x) {
//    point_mass = std::make_unique<PointMass>(
//        k /* The Spring Constant */,
//        m /* THe Mass of the Vertex */,
//        x /* The Initial Position of the Vertex */
//    );
//}

void Vertex::Render() {
    GLfloat vertices[] = {
        // Positions        // Colors
        0.5f,  -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
        0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f  // Top
    };


    glGenVertexArrays(1, &vertex_array_object);
    glGenBuffers(1, &vertex_buffer_object);

    glColor3f(1.0f, 1.0f, 1.0f);

    // Bind the Vertex Array Object
    glBindVertexArray(vertex_array_object);

    // Bind Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

    // Bind Vertices and Attribute Pointers
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure Position Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Configure Color Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT),
                          (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Unbind Vertex Array Object
    glBindVertexArray(0);

    is_init = true;
}
