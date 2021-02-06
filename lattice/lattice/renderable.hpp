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

#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

const glm::vec3 kRenderableColorRed = glm::vec3(1.f, 0.f, 0.f);

class Renderable {
  public:
    // Vertex Manager.
    GLuint vertex_array_object;

    // GPU Buffer Manager.
    GLuint vertex_buffer_object;

    Renderable(int size, const glm::vec3& color) : kSize(size), kColor(color) {}
    virtual ~Renderable() = default;

    virtual void Initialize() = 0;
    virtual void Translate(const glm::vec3& translation_vector) = 0;
    virtual inline void Render() = 0;
    virtual inline void Update(float dt) = 0;

  protected:
    // Always render from the first group, second group is for colors.
    constexpr static int kStartingIndex = 0;

    // The ending index, this is usually the object vector size * num objects
    constexpr static int kEndingIndex = 3;

    // The color of the shape
    const glm::vec3 kColor;

    // The "size" of the object, so for a triangle with size 1 it'd be:
    // [-1, -1, 0]  -> Bottom Left
    // [1, -1, 0]   -> Bottom Right
    // [0, 1, 0]    -> Top Center
    // Centered around the origin position in whatever class uses this.
    // When it's needed, I'll add an offset so we can diplay side-by-side
    const int kSize;

    bool is_init = false;

    // Vector of 4D points (x, y, z w = 1)
    std::vector<glm::vec4> vertices;

    // The shape constructed from the vertices
    std::vector<GLfloat> shape;
};

#endif /* renderable_hpp */
