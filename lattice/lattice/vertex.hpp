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

class Vertex {
  public:
    GLuint vertex_array_object;
    GLuint vertex_buffer_object;

    Vertex();
    ~Vertex() = default;

    void Initialize();
    void Transform(const glm::vec3& transform);
    void Render();
    void Update(float dt);

  private:
    void RebuildShape();

    const std::vector<float> RED{{1.0f, 0.0f, 0.0f}};

    // Vector of 4D points (x, y, z w = 1)
    std::vector<glm::vec4> vertices;

    // The shape constructed from the vertices
    std::vector<GLfloat> shape;
};

#endif /* vertex_hpp */
