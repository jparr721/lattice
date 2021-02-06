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
          position(glm::vec3(0, 0, 0)) {}
    Mass(float mass, glm::vec3 starting_position)
        : Renderable(1, kRenderableColorRed), kMass(mass),
          position(starting_position) {}
    ~Mass() = default;

    void CalculateMassForces(float dt);

    void Initialize() {
        // Construct our vertices centered around the origin position supplied
        // on construction
        const auto v1 = glm::vec4(position.x - kSize, position.y - kSize,
                                  position.z, 1.f); // Bottom Left
        const auto v2 = glm::vec4(position.x + kSize, position.y - kSize,
                                  position.z, 1.f); // Bottom Right
        const auto v3 = glm::vec4(position.x, position.y + kSize, position.z,
                                  1.f); // Top Center

        vertices = std::vector<glm::vec4>{{v1, v2, v3}};
        
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
        glVertexAttribPointer(1 /* Second Attribute Location */,
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
        // Bind Vertices and Attribute Pointers and Reserve the Memory in the GPU
        glBufferData(GL_ARRAY_BUFFER, shape.size() * sizeof(float),
                     static_cast<void*>(shape.data()), GL_STATIC_DRAW);
        
    }
    
    // TODO(@jparr721)
    inline void Update(float dt) { return; }
    
    
    void Translate(const glm::vec3& translation_vector) {
        auto transformation_matrix = glm::translate(glm::mat4(1.0f), translation_vector);
        
        // Translate all vertices via the translation matrix.
        for (int i = 0u; i < vertices.size(); ++i) {
            vertices[i] = transformation_matrix * vertices[i];
        }
        
        // Recompute our shape
        // TODO(@jparr721) - Can we do this on the same vector instead of copying between them?
        ComputeShapeWithColor();
    }

  private:
    // The damping constant to prevent explosiveness
    constexpr static float kDamping = 0.8f;

    // The mass of the... mass...
    const float kMass;

    // Gravitational constant vector, applies -9.81f pounds of negative force
    const glm::vec3 kGravity = glm::vec3(0.0f, -9.81f, 0.0f);

    // The position of the object currently with respect to time.
    glm::vec3 position;

    // The velocity the object is moving at with respect to time.
    glm::vec3 velocity = glm::vec3(0, 0, 0);

    // The acceleration of the object with respect to time.
    glm::vec3 acceleration = glm::vec3(0, 0, 0);

    void CalculateAcceleration();
    void ComputeShapeWithColor();
};

#endif /* mass_hpp */
