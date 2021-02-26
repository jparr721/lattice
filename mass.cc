#include "mass.h"
#include "spring.h"

#include <iostream>

void Mass::Initialize() {
    ComputeVertexPoints();

    colors.reserve(vertices.size());
    for (auto i = 0u; i < vertices.size(); ++i) {
        colors.push_back(kColor);
    }

    is_init = true;
}

void Mass::Update(float dt) {
    if (is_fixed) {
        return;
    }

    CalculateMassForces(dt);
    ComputeVertexPoints();
}

/**
 * Iterates the list of existing vertices, overwriting the positions
 * with the new vertex points offset by the current position
 */
void Mass::ComputeVertexPoints() {
    for (auto i = 0u; i < vertices.size(); ++i) {
        auto vertex_triple = vertices[i];
        vertices[i] = Eigen::Vector3f(vertex_triple.x() + position.x(),
                                      vertex_triple.y() + position.y(),
                                      vertex_triple.z() + position.z());
    }
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

void Mass::CalculateAcceleration() {
    // Mass of our object x gravity.
    const Eigen::Vector4f Fg = kGravity * mass_weight;

    Eigen::Vector4f Fs = Eigen::Vector4f(0.0, 0.0, 0.0, 0.0);

    for (const auto spring : springs) {
        Fs += spring->CalculateCurrentForce(shared_from_this());
    }

    acceleration = (Fg + Fs) / mass_weight;
}
