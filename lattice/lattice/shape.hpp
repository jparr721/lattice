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

#include <memory>
#include <utility>
#include <vector>

class Shape : public Renderable {
  public:
    Shape(std::vector<std::shared_ptr<Fixture>>& _fixtures, GLuint render_mode)
        : Renderable(render_mode), fixtures(_fixtures) {}
    
    unsigned int size() { return rendered_array_size; }

    inline void ComputeVertexPoints() {
        // First, initialize all of our vertex positions
        for (auto fixture : fixtures) {
            // Compute vertex points so that way it has an internal vertex
            // structure
            fixture->ComputeVertexPoints();

            const auto next_object_mapping_idx = element_buffer.size();

            // Fill out the elements object mapping.
            element_buffer.push_back(next_object_mapping_idx);

            // Fill out the bijection with the fixture size
            // Get the vertex position of the last vertex
            if (next_object_mapping_idx > 0) {
                const auto last_object_mapping_idx =
                    next_object_mapping_idx - 1;
                const auto highest_vertex_option =
                    buffer_mapping[last_object_mapping_idx].end;
                const auto bijection_pair =
                    std::make_pair(highest_vertex_option, fixture->size());

                const auto bijection_coordinate_bounds =
                    BijectionCoordinateBounds{
                        .start = highest_vertex_option,
                        .end = highest_vertex_option + fixture->size(),
                    };

                buffer_mapping.insert(std::make_pair(
                    next_object_mapping_idx, bijection_coordinate_bounds));
            }
        }
    }

    inline void ComputeShapes() {
        for (const auto& fixture : fixtures) {
            for (const auto vertex : fixture->Vertices()) {
                // Push back our vertices
                shapes.push_back(vertex.x);
                shapes.push_back(vertex.y);
                shapes.push_back(vertex.z);

                // Push back the color associated with this shape
                shapes.push_back(fixture->Color().r);
                shapes.push_back(fixture->Color().g);
                shapes.push_back(fixture->Color().b);
            }
        }
    }

    void Initialize() {
        ComputeVertexPoints();

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
        
        rendered_array_size = 0;
        
        for (auto fixture : fixtures) {
            rendered_array_size += fixture->size();
        }

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
    }

  private:
    std::vector<std::shared_ptr<Fixture>>& fixtures;
    
    unsigned int rendered_array_size;
};

#endif /* shape_hpp */
