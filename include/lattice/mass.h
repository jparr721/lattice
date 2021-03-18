#pragma once

#include <lattice/colors.h>

#include <string>

#include <QDebug>

class Spring;

class Mass : public std::enable_shared_from_this<Mass> {
  public:
    const bool fixed;
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

    // The vertex number of the node.
    int number;

    Mass(int number, bool fixed, Eigen::Vector3f color,
         Eigen::Vector3f starting_position)
        : position(starting_position), kColor(color), fixed(fixed),
          mass_weight(kMinimumMassValue), number(number) {}
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

    std::vector<Eigen::Vector3f> Colors() const { return colors; }

    auto size() const { return vertices.size(); }

  private:
    // The color of the mass object.
    const Eigen::Vector3f kColor;

    // The mass of the... mass...
    float mass_weight = kMinimumMassValue;

    // The original vertex positions
    std::vector<Eigen::Vector3f> original_positions;

    // Represents the colors mapped to each vertex.
    std::vector<Eigen::Vector3f> colors;

    // The springs that this mass is connected to.
    std::vector<std::shared_ptr<Spring>> springs;
};
