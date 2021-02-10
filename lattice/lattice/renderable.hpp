//
//  renderable.hpp
//  lattice
//
//  Renderable is an abstract class representing a renderable object. We render
//  the object as a triangle or a line based on the type that the inheritor
//  selects. Created by Jarred Parr on 2/4/21.
//

#ifndef renderable_hpp
#define renderable_hpp

#include <unordered_map>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class Renderable {
  public:
    // Vertex Memory Pointer.
    GLuint vertex_array_object;

    // GPU Buffer Pointer.
    GLuint vertex_buffer_object;

    Renderable(GLuint render_mode) : kRenderMode(render_mode) {}
    virtual ~Renderable() = default;

    // Initialize this renderable object.
    virtual void Initialize() = 0;

    // Render this renderable object.
    virtual inline void Render() = 0;

    // Update this renderable object.
    virtual inline void Update(float dt) = 0;

    GLuint RenderMode() { return kRenderMode; }

  protected:
    struct BijectionCoordinateBounds {
        unsigned long start;
        unsigned long end;
    };

    bool is_init = false;

    // The render mode of the object, like GL_TRIANGLES or GL_LINES
    const GLuint kRenderMode;

    // Vector of 4D points (x, y, z w = 1)
    std::vector<glm::vec4> vertices;

    // The shape constructed from the vertices
    std::vector<GLfloat> shapes;

    // The element buffer tracking the rendering order
    std::vector<GLuint> element_buffer;

    // Bijection mapping for sizes to their start-end coordinates
    // so, element 0 might have 6 values (rectangle), so it'd map
    // as 0 -> (Pair)(0, 5) inclusive.
    std::unordered_map<GLuint, BijectionCoordinateBounds> buffer_mapping;
};

#endif /* renderable_hpp */
