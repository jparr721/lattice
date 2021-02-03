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

#include <memory>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "point_mass.hpp"

class Vertex {
  public:
    GLuint vertex_array_object;
    GLuint vertex_buffer_object;

    Vertex() = default;
    //    Vertex(int k, int m, int x);
    ~Vertex() = default;

    void Render();
    void Update(float dt);

  private:
    //    std::unique_ptr<PointMass> point_mass;

    bool is_init = false;
};

#endif /* vertex_hpp */
