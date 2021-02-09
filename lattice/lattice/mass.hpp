//
//  mass.hpp
//  lattice
//
//  Represents a mass object as a square
//  Created by Jarred Parr on 2/4/21.
//

#ifndef mass_hpp
#define mass_hpp

#include "colors.hpp"
#include "fixture.hpp"

#include <glm/gtx/transform.hpp>

constexpr bool kFixedPosition = true;
constexpr bool kUnfixedPosition = false;

class Mass : public Fixture {
  public:
    // The mass of the... mass...
    const float kMass;

    const bool is_fixed;

    Mass()
        : Fixture(glm::vec4(0.f, 0.f, 0.f, 0.f), colors::kBlue), kMass(1),
          kSize(1), is_fixed(false) {}
    Mass(bool fixed, glm::vec4 starting_position)
        : Fixture(starting_position, colors::kBlue), kMass(1), kSize(1),
          is_fixed(fixed) {}
    Mass(float size, float mass, bool fixed, glm::vec3 color,
         glm::vec4 starting_position)
        : Fixture(starting_position, color), is_fixed(fixed), kMass(mass),
          kSize(size) {}
    ~Mass() = default;

    void CalculateMassForces(float dt);

    // TODO(@jparr721)
    inline void Update(float dt) {
        if (is_fixed) {
            return;
        }
        CalculateMassForces(dt);
        ComputeVertexPoints();
    }

    inline void ComputeVertexPoints() {
        // Construct our vertices centered around the origin position supplied
        // on construction
        const auto v1 = glm::vec4(position.x - kSize, position.y - kSize,
                                  position.z, 1.f); // Bottom Left
        const auto v2 = glm::vec4(position.x + kSize, position.y - kSize,
                                  position.z, 1.f); // Bottom Right
        const auto v3 = glm::vec4(position.x, position.y + kSize, position.z,
                                  1.f); // Top Center

        vertices = std::vector<glm::vec4>{{v1, v2, v3}};
    }

    void Translate(const glm::vec3& translation_vector) {
        auto transformation_matrix =
            glm::translate(glm::mat4(1.0f), translation_vector);

        // Translate all vertices via the translation matrix by just moving the
        // position
        position = transformation_matrix * position;

        // Recompute our vertices
        ComputeVertexPoints();
    }

    /**
        @brief Updates the acceleration of the mass object by some positive or
       negative delta value.
     */
    inline void ChangeAcceleration(const glm::vec4& delta) {
        acceleration += delta;
    }

  private:
    // The damping constant to prevent explosiveness
    constexpr static float kDamping = 0.8f;

    // The size of the object centered around the current position.
    const float kSize;

    // Gravitational constant vector, applies -9.81f
    // pounds of negative force
    const glm::vec4 kGravity = glm::vec4(0.0f, -9.81f, 0.0f, 1.0f);

    // The velocity the object is moving at with respect to time.
    glm::vec4 velocity = glm::vec4(0.f, 0.f, 0.f, 0.f);

    // The acceleration of the object with respect to time.
    glm::vec4 acceleration = glm::vec4(0.f, 0.f, 0.f, 0.f);

    void CalculateAcceleration();
};

#endif /* mass_hpp */
