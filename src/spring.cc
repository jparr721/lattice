#include <lattice/mass.h>
#include <lattice/spring.h>

void Spring::Initialize() {
    ComputeVertexPoints();

    for (auto i = 0u; i < vertices.size(); ++i) {
        colors.push_back(kColor);
    }

    left_mass->AddSpring(shared_from_this());
    right_mass->AddSpring(shared_from_this());
}

void Spring::ComputeVertexPoints() {
    const auto lpos = left_mass->position;
    const auto rpos = right_mass->position;

    const auto v1 = Eigen::Vector3f(lpos.x(), lpos.y(), lpos.z());
    const auto v2 = Eigen::Vector3f(rpos.x(), rpos.y(), rpos.z());
    const auto v3 = Eigen::Vector3f(rpos.x() + 0.1f, rpos.y() + 0.1f, rpos.z());
    const auto v4 = Eigen::Vector3f(rpos.x(), rpos.y(), rpos.z());
    const auto v5 = Eigen::Vector3f(lpos.x(), lpos.y(), lpos.z());
    const auto v6 = Eigen::Vector3f(lpos.x() + 0.1f, lpos.y() + 0.1f, lpos.z());

    vertices = std::vector<Eigen::Vector3f>{{v1, v2, v3, v4, v5, v6}};
}

void Spring::CalculateCurrentForce() {
    left_mass->force = kGravity;
    right_mass->force = kGravity;

    const Eigen::Vector3f lr_difference =
        left_mass->position - right_mass->position;

    const float rest_distance =
        (left_mass->rest_position - right_mass->rest_position).norm();
    const float distance = (lr_difference).norm();
    const float x = distance - rest_distance;

    const Eigen::Vector3f left_direction = (lr_difference).normalized();
    const Eigen::Vector3f right_direction = -left_direction;

    // F = -kx
#ifdef USE_NEURAL_NETWORK
    exit(0);
    const auto spring_force = socket->Predict(stiffness, x) * left_direction;
#else
    const auto spring_force = -(stiffness * x) * left_direction;
#endif

    // Record current displacement for NN training...
    left_mass->displacement = x;
    right_mass->displacement = x;

    left_mass->force += spring_force;
    right_mass->force += -spring_force;

    const float left_velocity_dir = left_mass->velocity.dot(left_direction);
    const float right_velocity_dir = right_mass->velocity.dot(right_direction);

    const Eigen::Vector3f left_force_difference =
        -damping_constant * left_direction * left_velocity_dir;
    const Eigen::Vector3f right_force_difference =
        -damping_constant * right_direction * right_velocity_dir;

    left_mass->force += left_force_difference;
    right_mass->force += right_force_difference;
}