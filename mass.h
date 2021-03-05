#pragma once

#include "colors.h"
#include "utility.h"

#include <string>

#include <QDebug>

class Spring;

class Mass : public std::enable_shared_from_this<Mass> {
  public:
    const bool is_fixed;
    constexpr static float kMinimumMassValue = 0.5f;
    constexpr static float kMaximumMassValue = 50.f;

    // The velocity the object is moving at with respect to time.
    Eigen::Vector3f velocity = Eigen::Vector3f::Zero();

    // The force of the object with respect to time.
    Eigen::Vector3f force = Eigen::Vector3f::Zero();

    // Represents the current position of the fixture.
    Eigen::Vector3f position;

    // The initial position of the mass object.
    Eigen::Vector3f rest_position;

    // Represents the vertices of the fixture.
    std::vector<Eigen::Vector3f> vertices;

    Mass(std::string name, bool fixed, Eigen::Vector3f color,
         Eigen::Vector3f starting_position)
        : position(starting_position), kColor(color), is_fixed(fixed),
          mass_weight(kMinimumMassValue), name(name) {}
    ~Mass() = default;

    // Class Initializers
    void Initialize();
    void Update(float dt);
    void ComputeVertexPoints();
    void CalculateMassForces(float dt);

    void AddSpring(std::shared_ptr<Spring> _spring) {
        springs.push_back(_spring);
    }

    // Trivial Setters
    void SetWeight(float value) { mass_weight = value; }

    // Complex Setters
    void Translate(const Eigen::Vector3f& translation_vector);

    // Trivial Getters
    float Weight() const { return mass_weight; }

    std::string Name() { return name; }

    std::vector<Eigen::Vector3f> Colors() const { return colors; }

    auto size() const { return vertices.size(); }

  private:
    // The initialization status of the fixture object.
    bool is_init = false;

    // The color of the mass object.
    const Eigen::Vector3f kColor;

    // The name of the mass node
    std::string name;

    // The mass of the... mass...
    float mass_weight = kMinimumMassValue;

    // The original vertex positions
    std::vector<Eigen::Vector3f> original_positions;

    // Represents the colors mapped to each vertex.
    std::vector<Eigen::Vector3f> colors;

    // The springs that this mass is connected to.
    std::vector<std::shared_ptr<Spring>> springs;

    void CalculateAcceleration();
};
