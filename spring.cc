#include "spring.h"
#include "mass.h"
#include "utility.h"

#include <iostream>

void Spring::Initialize() {
    ComputeVertexPoints();

    colors = std::vector<Eigen::Vector3f>{{kColor, kColor, kColor}};

    left_mass->AddSpring(shared_from_this());
    right_mass->AddSpring(shared_from_this());

    is_init = true;
}

void Spring::Update(float dt) { ApplySpringForces(); }

void Spring::ComputeVertexPoints() {
    const auto lpos = left_mass->Position();
    const auto rpos = right_mass->Position();

    const auto v1 = Eigen::Vector3f(lpos.x(),
                                    lpos.y(), lpos.z());
    const auto v2 = Eigen::Vector3f(rpos.x(),
                                    rpos.y(), rpos.z());
    const auto v3 = Eigen::Vector3f(rpos.x() + 0.01f,
                                    rpos.y(), rpos.z());

    vertices = std::vector<Eigen::Vector3f>{{v1, v2, v3}};
}

void Spring::ApplySpringForces() {
    const float current_spring_length = utility::EuclideanDistance(
        left_mass->Position(), right_mass->Position());

    const Eigen::Vector4f mass_norm =
        (left_mass->Position() - right_mass->Position()).normalized();

    force =
        stiffness * (current_spring_length - rest_length) * mass_norm.array();
    Eigen::Vector4f mass_acceleration_delta = force / right_mass->Weight();

    right_mass->ChangeAcceleration(mass_acceleration_delta);
    left_mass->ChangeAcceleration(mass_acceleration_delta);
}

Eigen::Vector4f Spring::CalculateCurrentForce(std::shared_ptr<Mass> ref) {
    const float current_spring_length = utility::EuclideanDistance(
        left_mass->Position(), right_mass->Position());

    const float hooke_scalar = stiffness * (current_spring_length - rest_length);

    const Eigen::Vector4f force_direction = (
        right_mass->Position() - left_mass->Position()).normalized();

    // Contraction/Expansion rate for damping force
    const float left_spring_response = left_mass->Velocity().dot(force_direction);
    const float right_spring_response = right_mass->Velocity().dot(force_direction);

    const float damping = damping_constant * (left_spring_response + right_spring_response);

    if (ref == left_mass) {
        force = (hooke_scalar + damping_constant) * force_direction;
        return force;
    } else {
        force = (-hooke_scalar + damping_constant) * force_direction;
        return force;
    }
}
