#pragma once

#include <lattice/generator.h>
#include <lattice/mass.h>
#include <lattice/spring.h>

#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

#include <Eigen/Dense>

class MassSpringSystem {
  public:
    constexpr static float kMinimumTimeStepChangeValue = 0.0001;
    constexpr static float kMaximumTimeStepChangeValue = 0.1f;

    std::string name;

    explicit MassSpringSystem(const generator::MSSConfig& config);
    ~MassSpringSystem() = default;

    void Update(float dt);
    void Reset();

    void ComputeShapes();
    void ComputeColors();

    // Stats
    void RecordSystemStatus();

    // Spring Mutators
    void SetSpringConstant(float value);
    void SetSpringRestLength(float value);
    void SetSpringDampingConstant(float value);

    // Mass Mutators
    void SetMassWeight(float value);

    // Training Data Signals
    std::unordered_map<int, Eigen::Vector3f> GetMassVelocities();
    std::unordered_map<int, Eigen::Vector3f> GetMassForces();

    std::vector<Eigen::Vector3f> Colors() { return colors; }
    std::vector<Eigen::Vector3f> Shapes() { return shapes; }

    auto size() { return springs.size() + masses.size(); }

  private:
    // The springs in the sim
    std::vector<std::shared_ptr<Spring>> springs;

    // The masses in the sim
    std::vector<std::shared_ptr<Mass>> masses;

    std::vector<Eigen::Vector3f> initial_positions;

    // Our constructed shapes in a flat list.
    std::vector<Eigen::Vector3f> shapes;

    // Our shapes' colors in a flat list.
    std::vector<Eigen::Vector3f> colors;

    std::unordered_map<int, int> mass_map;

    std::unordered_map<int, Eigen::Vector3f> mass_forces;
    std::unordered_map<int, Eigen::Vector3f> mass_velocities;

    generator::MSSConfig initial_conditions;

    void Redraw();
    void PreloadModelData();

    int ComputeY(int index, int total_masses, int rest_length);
};
