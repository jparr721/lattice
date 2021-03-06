#pragma once

#include <lattice/colors.h>
#include <lattice/socket.h>

#include <iostream>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

class Mass;

class Spring : public std::enable_shared_from_this<Spring> {
  public:
    constexpr static float kMinimumSpringConstantValue = 1.0f;
    constexpr static float kMinimumDampingValue = 0.5;
    constexpr static float kMinimumSpringRestLengthValue = 2.f;

    constexpr static float kMaximumSpringConstantValue = 500.0f;
    constexpr static float kMaximumDampingValue = 5.0f;
    constexpr static float kMaximumSpringRestLengthValue = 5.0f;

    // The spring k value.
    float k = kMinimumSpringConstantValue;

    // The resting length of the spring in the Y direction.
    float rest_length = kMinimumSpringRestLengthValue;

    // The spring calculation damping constant to prevent explosions.
    float damping = kMinimumDampingValue;

    // Represents the vertices of the fixture.
    std::vector<Eigen::Vector3f> vertices;

    std::shared_ptr<Socket> socket;

    Spring(std::shared_ptr<Mass>& _left_mass,
           std::shared_ptr<Mass>& _right_mass)
        : kColor(Colors::kGreen), left_mass(_left_mass),
          right_mass(_right_mass) {}

    Spring(std::shared_ptr<Socket> _socket, std::shared_ptr<Mass>& _left_mass,
           std::shared_ptr<Mass>& _right_mass)
        : kColor(Colors::kGreen), left_mass(_left_mass),
          right_mass(_right_mass), socket(std::move(_socket)) {}

    // Class Initializers and Mutators
    void Initialize();
    void ComputeVertexPoints();

    // Trivial Getters
    auto size() const { return vertices.size(); }

    float RestLength() const { return rest_length; }

    std::vector<Eigen::Vector3f> Colors() const { return colors; }

    // Non Trivial Getters
    void CalculateCurrentForce();

  private:
    // Gravitational constant vector, applies -9.81f
    // pounds of negative force
    const Eigen::Vector3f kGravity = Eigen::Vector3f(0.0f, -9.81f, 0.0f);

    const Eigen::Vector3f kColor;

    // Represents the colors mapped to each vertex.
    std::vector<Eigen::Vector3f> colors;

    // The left-side mass the spring is attached to.
    std::shared_ptr<Mass> left_mass;

    // The right-side mass the spring is attached to.
    std::shared_ptr<Mass> right_mass;
};
