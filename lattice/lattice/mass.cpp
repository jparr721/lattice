//
//  mass.cpp
//  lattice
//
//  Created by Jarred Parr on 2/4/21.
//

#include "mass.hpp"

void Mass::CalculateMassForces(float dt = 0.1f) {
    CalculateAcceleration();

    // Calculate new velocity with respect to time.
    velocity = velocity + acceleration * dt;

    // Calculate new position based on the current velocity with respect to
    // time.
    position = position + velocity * dt;
}

void Mass::CalculateAcceleration() {
    // Calculate damping dynamically based on how fast the object is moving.
    // Flip the sign of damping since we're adding it to the acceleration value.
    auto damping = (-1 * kDamping * velocity) / kMass;

    // Calculate our acceleration with gravity doing gravity things and damping
    // to prevent explosion.
    acceleration = acceleration + kGravity + damping;
}
