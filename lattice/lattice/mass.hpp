//
//  mass.hpp
//  lattice
//
//  Represents a mass object as a square
//  Created by Jarred Parr on 2/4/21.
//

#ifndef mass_hpp
#define mass_hpp

#include "renderable.hpp"

#include <glm/gtx/transform.hpp>

class Mass : public Renderable {
  public:
    Mass()
        : Renderable(1, kRenderableColorRed), kMass(1),
          position(glm::vec4(0, 0, 0, 1)) {}
    Mass(float mass, glm::vec4 starting_position)
        : Renderable(1, kRenderableColorRed), kMass(mass),
          position(starting_position) {}
    ~Mass() = default;

    void CalculateMassForces(float dt);

    void Initialize() {
        ComputeVertexPoints();

        // Build our internal shape object
        ComputeShapeWithColor();

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
        glBufferData(GL_ARRAY_BUFFER, shape.size() * sizeof(float),
                     static_cast<void*>(shape.data()), GL_STATIC_DRAW);
    }

    // TODO(@jparr721)
    inline void Update(float dt) { return; }

    void Translate(const glm::vec3& translation_vector) {
        auto transformation_matrix =
            glm::translate(glm::mat4(1.0f), translation_vector);

        // Translate all vertices via the translation matrix by just moving the
        // position
        position = transformation_matrix * position;

        // Recompute our vertices
        ComputeVertexPoints();

        // Recompute our shape
        ComputeShapeWithColor();
    }

  private:
    // The damping constant to prevent explosiveness
    constexpr static float kDamping = 0.8f;

    // The mass of the... mass...
    const float kMass;

    // Gravitational constant vector, applies -9.81f pounds of negative force
    const glm::vec4 kGravity = glm::vec4(0.0f, -9.81f, 0.0f, 1.0f);

    // The position of the object currently with respect to time.
    glm::vec4 position;

    // The velocity the object is moving at with respect to time.
    glm::vec4 velocity = glm::vec4(0.f, 0.f, 0.f, 0.f);

    // The acceleration of the object with respect to time.
    glm::vec4 acceleration = glm::vec4(0.f, 0.f, 0.f, 0.f);

    void CalculateAcceleration();
    void ComputeShapeWithColor();
    void ComputeVertexPoints();
};

#endif /* mass_hpp */
