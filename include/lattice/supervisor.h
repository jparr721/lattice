#pragma once

#include <lattice/configuration.h>
#include <lattice/mass_spring_system.h>

#include <memory>
#include <vector>

class Supervisor {
  public:
    constexpr static float kMinimumTimeStepChangeValue = 0.0001;

    float timestep_size = kMinimumTimeStepChangeValue;

    // Shape density
    const int density;

    // VBO Shapes
    std::vector<Eigen::Vector3f> shapes;

    // VBO Colors
    std::vector<Eigen::Vector3f> colors;

    std::vector<std::shared_ptr<MassSpringSystem>> simulations;

    explicit Supervisor(const MSSConfig& config);
    ~Supervisor() = default;

    // Setters
    void SetMassWeight(float value);
    void SetSpringConstant(float value);
    void SetSpringDampingConstant(float value);
    void SetSpringRestLength(float value);
    void SetTimeStep(float value) { timestep_size = value; }

    // Update
    void Update();

    // Reset
    void Reset();

    // Vertex Buffer Functions
    void RecompileVertexBuffer();

    // Samplers
    std::unordered_map<std::string, std::unordered_map<int, Eigen::Vector3f>>
    SampleMassVelocities();

    std::unordered_map<std::string, std::unordered_map<int, Eigen::Vector3f>>
    SampleMassForces();

  private:
    std::unordered_map<std::string, std::unordered_map<int, Eigen::Vector3f>>
        current_mass_velocities;

    std::unordered_map<std::string, std::unordered_map<int, Eigen::Vector3f>>
        current_mass_forces;
};
