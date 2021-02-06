//
//  mass.cpp
//  lattice
//
//  Created by Jarred Parr on 2/4/21.
//

#include "mass.hpp"

void Mass::ComputeShapeWithColor() {
    // THIS BREAKS IF SHAPE IS < 3 VECTORS
    shape = std::vector<GLfloat>((std::pow(vertices.size(), 2) +
                  std::pow(sizeof(kColor) / sizeof(float), 2)), 0);

    // TODO(@jparr721) - If this stays at a static 3-point shape, we can force
    // clang to unroll this via: #pragma clang loop unroll_count(3) This will
    // speed up computation some.
    for (int i = 0; i < vertices.size(); ++i) {
        auto vertex = vertices[i];

        int j = i * 6;

        // Insert Vertex Positions
        shape[j] = vertex.x;
        shape[j + 1] = vertex.y;
        shape[j + 2] = vertex.z;

        // Insert Colors
        shape[j + 3] = kColor.r;
        shape[j + 4] = kColor.g;
        shape[j + 5] = kColor.b;
    }
}

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
