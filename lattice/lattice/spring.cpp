//
//  spring.cpp
//  lattice
//
//  Created by Jarred Parr on 2/6/21.
//

#include "spring.hpp"

void Spring::ApplySpringForces() {
    const float current_spring_length =
        glm::distance(left_mass->Position(), right_mass->Position());

    // Calculate the vector norm so we can express hooke's law as a directional
    // vector
    const glm::vec4 mass_norm =
        glm::normalize(left_mass->Position() - right_mass->Position());

    const glm::vec3 force =
        kStiffness * (current_spring_length - kLength) * mass_norm;
    const glm::vec4 mass_acceleration_delta =
        glm::vec4(force / right_mass->kMass, 1.f);

    // Update acceleration for the right mass. Right now, this is all that's
    // needed, for later sims, we will need to change this to support multi-mass
    // updates (which should be easy). We are only updating the right mass
    // because the left mass is a fixed-point anchor for the spring to hook to.
    right_mass->ChangeAcceleration(mass_acceleration_delta);
}
