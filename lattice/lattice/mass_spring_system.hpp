//
//  shape.hpp
//  lattice
//
//  Created by Jarred Parr on 2/7/21.
//

#ifndef shape_hpp
#define shape_hpp

#include "fixture.hpp"
#include "renderable.hpp"

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

class MassSpringSystem : public Renderable {
  public:
    MassSpringSystem(std::vector<std::shared_ptr<Fixture>>& _fixtures,
                     GLuint render_mode)
        : Renderable(render_mode), fixtures(_fixtures) {}

    void MoveFixtureAtIndex(int index, const glm::vec3& translation_vector);

    unsigned int size() { return rendered_array_size; }

    inline void ComputeVertexPoints() {
        for (auto fixture : fixtures) {
            // Compute vertex points so that way it has an internal vertex
            // structure
            fixture->ComputeVertexPoints();
        }
    }

    inline void ComputeShapes() {
        for (int i = 0; i < fixtures.size(); ++i) {
            // Only works when rendering triangles!!!
            int starting_index = i * 18; // 6 vertices x 3 points
            const auto fixture = fixtures[i];
            const auto vertices = fixture->Vertices();

            for (int j = 0; j < vertices.size(); ++j) {
                const auto vertex = vertices[j];
                const auto color = fixture->Color();

                // Our j index mapping our stride
                int k = starting_index + j * 6; // 6 spaces between entries

                // Update our vertices
                shapes[k] = vertex.x;
                shapes[k + 1] = vertex.y;
                shapes[k + 2] = vertex.z;

                // Update the color associated with this shape
                shapes[k + 3] = color.r;
                shapes[k + 4] = color.g;
                shapes[k + 5] = color.b;
            }
        }
    }

    void Initialize() {
        ComputeVertexPoints();

        // Calculate our total shape size of vertices
        for (auto fixture : fixtures) {
            rendered_array_size += fixture->size();
        }

        n_vertices = rendered_array_size * 6;

        shapes = std::vector<GLfloat>(n_vertices, 0.0);

        // Build our internal shape object
        ComputeShapes();

        // Load the being-used vertex objects into memory for later use
        glGenVertexArrays(1, &vertex_array_object);

        // Initialize our class-owned vertex buffer on the GPU
        glGenBuffers(1, &vertex_buffer_object);

        // Bind the Vertex Array Object to track the vertices
        glBindVertexArray(vertex_array_object);

        // Bind Vertex Buffer Object
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

        // Configure Position Attribute, pass up to the shader
        glVertexAttribPointer(
            0 /* One Vertex Buffer, first attribute location */,
            3 /* Using vec3 to represent the positions of vertices */,
            GL_FLOAT /* Buffer type is float */, GL_FALSE, 6 * sizeof(GL_FLOAT),
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

    inline void Render() {
        assert(is_init);

        // Bind Vertices and Attribute Pointers and Reserve the Memory in the
        // GPU
        glBufferData(GL_ARRAY_BUFFER, shapes.size() * sizeof(float),
                     static_cast<void*>(shapes.data()), GL_STATIC_DRAW);
    }

    inline void Update(float dt) {
        for (auto fixture : fixtures) {
            fixture->Update(dt);
        }
        ComputeVertexPoints();
        ComputeShapes();
    }

  private:
    std::vector<std::shared_ptr<Fixture>>& fixtures;

    unsigned int rendered_array_size = 0;

    unsigned int n_vertices = 0;
};

#endif /* shape_hpp */
