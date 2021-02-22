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

Eigen::Vector4f Spring::CalculateCurrentForce(std::shared_ptr<Mass> ref) {
    const float current_spring_length = utility::EuclideanDistance(
        left_mass->Position(), right_mass->Position());

    const float hooke_scalar = stiffness * (current_spring_length - rest_length);

    const Eigen::Vector4f force_direction = (
        right_mass->Position() - left_mass->Position()).normalized();

    // Contraction/Expansion rate for damping force with respect to the direction its applied
    const float left_spring_response = left_mass->Velocity().dot(force_direction);
    const float right_spring_response = right_mass->Velocity().dot(force_direction);

    // Multiply our damping by the sum of the spring forces in either direction.
    const float damping = damping_constant * (left_spring_response + right_spring_response);

    if (ref == left_mass) {
        force = (hooke_scalar + damping_constant) * force_direction;
        return force;
    } else {
        force = (-hooke_scalar + damping_constant) * force_direction;
        return force;
    }
}
