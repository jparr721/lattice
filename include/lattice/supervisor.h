#pragma once

#include <lattice/configuration.h>
#include <lattice/mass_spring_system.h>

#include <memory>
#include <vector>

struct SupervisorParameters {
    float mass;
    float k;
    float damping;
    float rest_length;
    float time_step;
};

class Supervisor {
  public:
    constexpr static float kMinimumTimeStepChangeValue = 0.0001;

    float time_step_size = kMinimumTimeStepChangeValue;

    // Shape density
    const int density;

    // VBO Shapes
    std::vector<Eigen::Vector3f> shapes;

    // VBO Colors
    std::vector<Eigen::Vector3f> colors;

    std::vector<std::shared_ptr<MassSpringSystem>> simulations;

    explicit Supervisor(const MSSConfig& config);
    Supervisor(const MSSConfig& config, const SupervisorParameters& params);
    ~Supervisor() = default;



    // Setters
    void Initialize(const MSSConfig& config);
    void SetMassWeight(float value);
    void SetSpringConstant(float value);
    void SetSpringDampingConstant(float value);
    void SetSpringRestLength(float value);
    void SetTimeStep(float value) { time_step_size = value; }

    // Getters
    [[nodiscard]] SupervisorParameters CurrentParameters() const;
    [[nodiscard]] float GetMassWeight() const;
    [[nodiscard]] float GetSpringConstant() const;
    [[nodiscard]] float GetSpringDampingConstant() const;
    [[nodiscard]] float GetSpringRestLength() const;
    [[nodiscard]] float GetTimeStep() const;

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
