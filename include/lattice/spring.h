#pragma once

#include "colors.h"

#include <iostream>
#include <memory>
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

    // Represents the vertices of the fixture.
    std::vector<Eigen::Vector3f> vertices;

    Spring(Eigen::Vector3f color, std::shared_ptr<Mass>& _left_mass,
           std::shared_ptr<Mass>& _right_mass)
        : kColor(colors::kGreen), left_mass(_left_mass),
          right_mass(_right_mass) {}

    // Class Initializers and Mutators
    void Initialize();
    void ComputeVertexPoints();

    // Setters
    void SetStiffness(float value) { stiffness = value; }
    void SetRestLength(float value) { rest_length = value; }
    void SetDampingConstant(float value) { damping_constant = value; }

    // Trivial Getters
    auto size() const { return vertices.size(); }

    float RestLength() const { return rest_length; }

    Eigen::Vector3f Force() const { return force; }

    std::vector<Eigen::Vector3f> Colors() const { return colors; }

    // Non Trivial Getters
    void CalculateCurrentForce();

  private:
    // Gravitational constant vector, applies -9.81f
    // pounds of negative force
    const Eigen::Vector3f kGravity = Eigen::Vector3f(0.0f, -9.81f, 0.0f);

    // The initialization status of the fixture object.
    bool is_init = false;

    // The spring k value.
    float stiffness = kMinimumSpringConstantValue;

    // The resting length of the spring in the Y direction.
    float rest_length = kMinimumSpringRestLengthValue;

    // The spring calculation damping constant to prevent explosions.
    float damping_constant = kMinimumDampingValue;

    const Eigen::Vector3f kColor;

    // The force that the spring is exerting;
    Eigen::Vector3f force;

    // Represents the colors mapped to each vertex.
    std::vector<Eigen::Vector3f> colors;

    // The left-side mass the spring is attached to.
    std::shared_ptr<Mass> left_mass;

    // The right-side mass the spring is attached to.
    std::shared_ptr<Mass> right_mass;

    void CalculateDampingForce();
};
