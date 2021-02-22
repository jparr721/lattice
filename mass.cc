#include "mass.h"
#include "spring.h"

#include <iostream>

void Mass::Initialize() {
    ComputeVertexPoints();

    colors = std::vector<Eigen::Vector3f>{{kColor, kColor, kColor}};

    is_init = true;
}

void Mass::Update(float dt) {
    if (is_fixed) {
        return;
    }

    CalculateMassForces(dt);
    ComputeVertexPoints();
}

void Mass::ComputeVertexPoints() {
    // Construct our vertices centered around the origin position supplied
    // on construction
    const auto v1 =
        Eigen::Vector3f(position(utility::VectorPosition::X) - mass_size,
                        position(utility::VectorPosition::Y) - mass_size,
                        position(utility::VectorPosition::Z)); // Bottom Left
    const auto v2 =
        Eigen::Vector3f(position(utility::VectorPosition::X) + mass_size,
                        position(utility::VectorPosition::Y) - mass_size,
                        position(utility::VectorPosition::Z)); // Bottom Right
    const auto v3 =
        Eigen::Vector3f(position(utility::VectorPosition::X),
                        position(utility::VectorPosition::Y) + mass_size,
                        position(utility::VectorPosition::Z)); // Top Center

    vertices = std::vector<Eigen::Vector3f>{{v1, v2, v3}};
}

void Mass::Translate(const Eigen::Vector3f& translation_vector) {
    Eigen::Matrix4f translation_matrix;
    translation_matrix.setIdentity();
    translation_matrix.block<3, 1>(0, 3) = translation_vector;

    position = translation_matrix * position;

    // Recompute our vertices
    ComputeVertexPoints();
}

void Mass::CalculateMassForces(float dt) {
    // We don't want to calculate for fixed masses, that would be bad.
    assert(!is_fixed && "Mass is fixed");
    CalculateAcceleration();

    // Calculate new velocity with respect to time.
    velocity += acceleration * dt;

    // Calculate new position based on the current velocity with respect to
    // time.
    position += velocity * 0.1 * dt;
}

void Mass::ChangeAcceleration(const Eigen::Vector4f& delta) {
    acceleration += delta;
}

void Mass::CalculateAcceleration() {
    // Mass of our object x gravity.
    const Eigen::Vector4f Fg = kGravity * mass_weight;

    Eigen::Vector4f Fs = Eigen::Vector4f(0.0, 0.0, 0.0, 0.0);

    for (const auto spring : springs) {
        Fs += spring->CalculateCurrentForce(shared_from_this());
    }

    acceleration = (Fg + Fs) / mass_weight;
}
