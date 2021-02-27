#include "mass.h"
#include "obj_loader.h"
#include "spring.h"

#include <iostream>

void Mass::Initialize() {
    if (!LoadObjFile("sim_specs/cube.obj", original_positions)) {
        exit(EXIT_FAILURE);
    }

    vertices = std::vector<Eigen::Vector3f>(original_positions.size(),
                                            Eigen::Vector3f(0.f, 0.f, 0.f));

    ComputeVertexPoints();

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
}

void Mass::ComputeVertexPoints() {

    // Build our vertices and "scoot" them to their starting positions.
    for (auto i = 0u; i < original_positions.size(); ++i) {
        const auto vertex = original_positions[i];
        vertices[i] = Eigen::Vector3f(vertex.x() + position.x(),
                                      vertex.y() + position.y(), vertex.z());
    }
}

void Mass::Translate(const Eigen::Vector3f& translation_vector) {
    for (auto i = 0u; i < vertices.size(); ++i) {
        auto vertex = vertices[i];
        Eigen::Matrix4f translation_matrix;
        translation_matrix.setIdentity();
        translation_matrix.block<3, 1>(0, 3) = translation_vector;

        Eigen::Vector4f transform =
            translation_matrix *
            Eigen::Vector4f(vertex.x(), vertex.y(), vertex.z(), 1.f);
        vertices[i] =
            Eigen::Vector3f(transform.x(), transform.y(), transform.z());
    }
}

void Mass::CalculateMassForces(float dt) {
    // We don't want to calculate for fixed masses, that would be bad.
    assert(!is_fixed && "Mass is fixed");
    CalculateAcceleration();

    auto old_pos = position;

    // Calculate new velocity with respect to time.
    velocity += acceleration * dt;

    // Calculate new position based on the current velocity with respect to
    // time.
    position += velocity * 0.5 * dt;

    auto d_position = position - old_pos;
    Translate(Eigen::Vector3f(d_position.x(), d_position.y(), d_position.z()));
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
