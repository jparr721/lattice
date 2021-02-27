#include "spring.h"
#include "mass.h"
#include "utility.h"

#include <iostream>

void Spring::Initialize() {
    ComputeVertexPoints();

    for (auto i = 0u; i < vertices.size(); ++i) {
        colors.push_back(kColor);
    }

    left_mass->AddSpring(shared_from_this());
    right_mass->AddSpring(shared_from_this());

    is_init = true;
}

void Spring::ComputeVertexPoints() {
    const auto lpos = left_mass->Position();
    const auto rpos = right_mass->Position();

    const auto v1 = Eigen::Vector3f(lpos.x(), lpos.y(), lpos.z());
    const auto v2 = Eigen::Vector3f(rpos.x(), rpos.y(), rpos.z());
    const auto v3 = Eigen::Vector3f(rpos.x() + 0.1f, rpos.y() + 0.1f, rpos.z());
    const auto v4 = Eigen::Vector3f(rpos.x(), rpos.y(), rpos.z());
    const auto v5 = Eigen::Vector3f(lpos.x(), lpos.y(), lpos.z());
    const auto v6 = Eigen::Vector3f(lpos.x() + 0.1f, lpos.y() + 0.1f, lpos.z());

    vertices = std::vector<Eigen::Vector3f>{{v1, v2, v3, v4, v5, v6}};
}

Eigen::Vector4f Spring::CalculateCurrentForce(std::shared_ptr<Mass> ref) {
    const float current_spring_length =
        (left_mass->Position() - right_mass->Position()).norm();
    const float x = current_spring_length - rest_length;

    const Eigen::Vector4f force_direction =
        (right_mass->Position() - left_mass->Position()).normalized();

    const float spring_force = stiffness * x;

    // Contraction/Expansion rate for damping force with respect to the
    // direction its applied
    const float left_spring_response =
        left_mass->Velocity().dot(force_direction);
    const float right_spring_response =
        right_mass->Velocity().dot(force_direction);

    // Multiply our damping by the sum of the spring forces in either direction.
    const float damping =
        damping_constant * (left_spring_response + right_spring_response);

    if (ref == left_mass) {
        force = (spring_force + damping_constant) * force_direction;
        return force;
    } else {
        force = (-spring_force + damping_constant) * force_direction;
        return force;
    }
}
