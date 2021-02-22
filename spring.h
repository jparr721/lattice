#pragma once

#include "colors.h"
#include "utility.h"

#include <iostream>
#include <memory>
#include <vector>

class Mass;

class Spring : public std::enable_shared_from_this<Spring> {
  public:
    Spring(float stiffness, float resting_length, Eigen::Vector3f color,
           std::shared_ptr<Mass>& _left_mass,
           std::shared_ptr<Mass>& _right_mass)
        : kColor(colors::kGreen), stiffness(stiffness),
          rest_length(resting_length), left_mass(_left_mass),
          right_mass(_right_mass) {}

    // Class Initializers and Mutators
    void Initialize();
    void ComputeVertexPoints();

    // Setters
    void SetStiffness(float value) { stiffness = value; }
    void SetRestLength(float value) { rest_length = value; }

    // Trivial Getters
    auto size() { return vertices.size(); }
    Eigen::Vector4f Force() const { return force; }
    std::vector<Eigen::Vector3f> Vertices() { return vertices; }
    std::vector<Eigen::Vector3f> Colors() { return colors; }

    // Non Trivial Getters
    Eigen::Vector4f CalculateCurrentForce(std::shared_ptr<Mass> ref);

  private:
    // The initialization status of the fixture object.
    bool is_init = false;

    // The spring k value.
    float stiffness;

    // The resting length of the spring in the Y direction.
    float rest_length;

    // The spring calculation damping constant to prevent explosions.
    float damping_constant = 0.01;

    const Eigen::Vector3f kColor;

    // The force that the spring is exerting;
    Eigen::Vector4f force;

    // Represents the vertices of the fixture.
    std::vector<Eigen::Vector3f> vertices;

    // Represents the colors mapped to each vertex.
    std::vector<Eigen::Vector3f> colors;

    // The left-side mass the spring is attached to.
    std::shared_ptr<Mass> left_mass;

    // The right-side mass the spring is attached to.
    std::shared_ptr<Mass> right_mass;
};
