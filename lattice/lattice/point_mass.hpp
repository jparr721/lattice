//
//  point_mass.hpp
//  lattice
//
//  Point Mass sidecars any rigid mass object and calculates
//  all of the forces on its behalf to update.
//  Created by Jarred Parr on 2/1/21.
//

#ifndef point_mass_hpp
#define point_mass_hpp

#include <glm/glm.hpp>

struct PointMass {
    PointMass()
        : spring_constant(1), mass(1), position(glm::vec3(0, 0, 0)),
          velocity(glm::vec3(0, 0, 0)), acceleration(glm::vec3(0, 0, 0)) {}
    
    PointMass(float k, float m, glm::vec3 pos, glm::vec3 v, glm::vec3 acc)
        : spring_constant(k), mass(m), position(pos), velocity(v),
          acceleration(acc) {}
    ~PointMass() = default;

    float update(float dt);

    // The position of the object currently with respect to time.
    glm::vec3 position;

    // The velocity the object is moving at with respect to time.
    glm::vec3 velocity;

    // The acceleration of the object with respect to time.
    glm::vec3 acceleration;

  private:
    // The k value in hooke's law
    const float spring_constant;

    // The mass of this vertex
    const float mass;
};

#endif /* point_mass_hpp */
