//
//  vertex.hpp
//  lattice
//
//  Vertex represents a mass object at a vertex between n springs.
//  vertices are updated on each time step.
//  Created by Jarred Parr on 2/2/21.
//

#ifndef vertex_hpp
#define vertex_hpp

#include <vector>

#include <memory>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "point_mass.hpp"

class Vertex {
  public:
    GLuint vertex_array_object;
    GLuint vertex_buffer_object;

    Vertex() = default;
    //    Vertex(int k, int m, int x);
    ~Vertex() = default;

    void Transform(const glm::vec3& transform);
    void Render();
    void Update(float dt);

  private:
    const std::vector<float> RED{{ 1.0f, 0.0f, 0.0f }};
    
    // Set up vertices representing the triangle points (x, y, z w = 1)
    glm::vec4 v1;
    glm::vec4 v2;
    glm::vec4 v3;
    
    std::vector<GLfloat> vertices;

    bool is_init = false;
};

#endif /* vertex_hpp */
