#pragma once

#include <lattice/mass_spring_system.h>
#include <lattice/shape_spec.h>

#include <memory>
#include <vector>

class Supervisor {
  public:
    constexpr static float kMinimumTimeStepChangeValue = 0.0001;
    constexpr static float kMaximumTimeStepChangeValue = 0.1f;

    // VBO Shapes
    std::vector<Eigen::Vector3f> shapes;

    // VBO Colors
    std::vector<Eigen::Vector3f> colors;

    std::vector<std::shared_ptr<MassSpringSystem>> simulations;

    explicit Supervisor(std::shared_ptr<ShapeSpec>& shape_spec);
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
    Eigen::Vector3f SampleMassVelocity();
    Eigen::Vector3f SampleMassForce();

  private:
    float timestep_size = kMinimumTimeStepChangeValue;
};
