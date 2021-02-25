#pragma once

#include "mass.h"
#include "spring.h"

#include <optional>
#include <string>
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

    void AddSpring(const std::shared_ptr<Spring>& spring);
    void AddMass(const std::shared_ptr<Mass>& mass);

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

    // Mass Plottable Getters
    Eigen::Vector4f GetFirstMovingMassVelocity();
    Eigen::Vector4f GetFirstMovingMassAcceleration();

    // Spring Plottable Getters
    Eigen::Vector4f GetFirstSpringForce();

    std::vector<Eigen::Vector3f> Colors() { return colors; }
    std::vector<Eigen::Vector3f> Shapes() { return shapes; }

    auto size() { return springs.size() + masses.size(); }

    // TODO(@jparr721) - Change when refactoring later!!!
    void TranslateTopGroup(const Eigen::Vector3f& direction);
    void TranslateBottomGroup(const Eigen::Vector3f& direction);

  private:
    bool is_init = false;

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

    // TODO(@jparr721) - This is not a good way to do this which scales well.
    std::vector<std::shared_ptr<Mass>> top_masses;
    std::vector<std::shared_ptr<Mass>> bottom_masses;

    void Redraw();
};
