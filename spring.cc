#include "spring.h"
#include "mass.h"
#include "utility.h"

void Spring::Initialize() {
    ComputeVertexPoints();

    colors = std::vector<Eigen::Vector3f>{{kColor, kColor, kColor}};

    is_init = true;
}

void Spring::Update(float dt) { ApplySpringForces(); }

void Spring::ComputeVertexPoints() {
    const auto lpos = left_mass->Position();
    const auto rpos = right_mass->Position();

    const auto v1 = Eigen::Vector3f(lpos(utility::VectorPosition::X),
                                    lpos(utility::VectorPosition::Y), 0.f);
    const auto v2 = Eigen::Vector3f(rpos(utility::VectorPosition::X),
                                    rpos(utility::VectorPosition::Y), 0.f);
    const auto v3 = Eigen::Vector3f(rpos(utility::VectorPosition::X) + 0.01f,
                                    rpos(utility::VectorPosition::Y), 0.f);

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
}
