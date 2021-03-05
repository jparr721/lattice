#pragma once

#include "mass.h"
#include "shape_spec.h"
#include "spring.h"

#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

#include <Eigen/Dense>

class MassSpringSystem {
  public:
    constexpr static float kMinimumTimeStepChangeValue = 0.0001;
    constexpr static float kMaximumTimeStepChangeValue = 0.1f;

    MassSpringSystem();
    ~MassSpringSystem() = default;

    void Update();
    void Reset();

    void ComputeShapes();
    void ComputeColors();

    // Spring Mutators
    void SetSpringStiffness(float value);
    void SetSpringRestLength(float value);
    void SetSpringDampingConstant(float value);

    // Mass Mutators
    void SetMassWeight(float value);

    // Time Step Mutators
    void SetTimeStep(float value) { timestep_size = value; }

    // Mass Getter
    std::optional<std::shared_ptr<Mass>> GetMassByName(const std::string& name);

    // Mass Plottable Getters
    Eigen::Vector3f GetFirstMovingMassVelocity();
    Eigen::Vector3f GetFirstMovingMassForce();

    // Spring Plottable Getters
    Eigen::Vector3f GetFirstSpringForce();

    std::vector<Eigen::Vector3f> Colors() { return colors; }
    std::vector<Eigen::Vector3f> Shapes() { return shapes; }

    auto size() { return springs.size() + masses.size(); }

  private:
    float timestep_size = kMinimumTimeStepChangeValue;

    // The springs in the sim
    std::vector<std::shared_ptr<Spring>> springs;

    // The masses in the sim
    std::vector<std::shared_ptr<Mass>> masses;

    std::vector<Eigen::Vector3f> initial_positions;

    // Our constructed shapes in a flat list.
    std::vector<Eigen::Vector3f> shapes;

    // Our shapes' colors in a flat list.
    std::vector<Eigen::Vector3f> colors;

    std::unordered_map<std::string, int> mass_map;

    std::unique_ptr<ShapeSpec> initial_conditions;

    void Redraw();

    int ComputeY(int index, int total_masses, int rest_length);
};
