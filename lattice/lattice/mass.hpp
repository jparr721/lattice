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

#include <memory>

#include <glm/glm.hpp>

class Mass : private Renderable {
  public:
    Mass()
        : Renderable(1, kRenderableColorRed, GL_TRIANGLES), kMass(1),
          position(glm::vec3(0, 0, 0)) {}
    Mass(float mass, glm::vec3 starting_position)
        : Renderable(1, kRenderableColorRed, GL_TRIANGLES), kMass(mass),
          position(starting_position) {}
    ~Mass() = default;

    void CalculateMassForces(float dt);

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
};

#endif /* mass_hpp */
