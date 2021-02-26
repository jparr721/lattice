#pragma once

#include "mass.h"
#include "spring.h"

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <Eigen/Dense>

class MassSpringSystem {
  public:
    //  Minimum Values
    constexpr static float kMinimumMassValue = 10.5f;
    constexpr static float kMinimumSpringConstantValue = 1.0f;
    constexpr static float kMinimumDampingValue = 0.5;
    constexpr static float kMinimumSpringRestLengthValue = 0.5f;
    constexpr static float kMinimumTimeStepChangeValue = 0.0001;

    //  Maxmum Values
    constexpr static float kMaximumMassValue = 10.0f;
    constexpr static float kMaximumSpringConstantValue = 50.0f;
    constexpr static float kMaximumDampingValue = 5.0f;
    constexpr static float kMaximumSpringRestLengthValue = 10.0f;
    constexpr static float kMaximumTimeStepChangeValue = 0.1f;

    MassSpringSystem(const std::string& _name = "sim_1");
    ~MassSpringSystem() = default;

    void Update();
    void Reset();

    void ComputeVertexPoints();
    void ComputeShapes();
    void ComputeColors();

    // Spring Mutators
    void SetSpringStiffness(float value);
    void SetSpringRestLength(float value);

    // Mass Mutators
    void SetMassWeight(float value);
    void SetMassDampingConstant(float value);

    // Time Step Mutators
    void SetTimeStep(float value) { timestep_size = value; }

    // Mass Getter
    std::optional<std::shared_ptr<Mass>> GetMassByName(const std::string& name);

    // TODO(@jparr721) - Make these work with all non-fixed masses
    // Mass Plottable Getters
    Eigen::Vector4f GetFirstMovingMassVelocity();
    Eigen::Vector4f GetFirstMovingMassAcceleration();

    // TODO(@jparr721) - Make these work with all springs
    // Spring Plottable Getters
    Eigen::Vector4f GetFirstSpringForce();

    std::vector<Eigen::Vector3f> Colors() { return colors; }
    std::vector<Eigen::Vector3f> Shapes() { return shapes; }

    auto size() { return springs.size() + masses.size(); }

  private:
    std::string name;

    float timestep_size = kMinimumTimeStepChangeValue;

    // The springs in the sim
    std::vector<std::shared_ptr<Spring>> springs;

    // The masses in the sim
    std::vector<std::shared_ptr<Mass>> masses;

    // Our constructed shapes in a flat list.
    std::vector<Eigen::Vector3f> shapes;

    // Our shapes' colors in a flat list.
    std::vector<Eigen::Vector3f> colors;

    std::pair<int, int> ComputeStartingPosition(int i);
};
