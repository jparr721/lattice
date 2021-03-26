#pragma once

#include <lattice/colors.h>

#include <string>

#include <QDebug>
#include <utility>

class Spring;

class Mass : public std::enable_shared_from_this<Mass> {
  public:
    constexpr static float kMinimumMassValue = 0.5f;
    constexpr static float kMaximumMassValue = 50.f;

    float weight = kMinimumMassValue;

    const bool fixed;

    // Spring->Mass displacement
    float displacement = 0.f;

    // The springs that this mass is connected to.
    std::vector<std::shared_ptr<Spring>> springs;

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
        : position(std::move(starting_position)), kColor(std::move(color)), fixed(fixed), weight(kMinimumMassValue), number(number) {}
    ~Mass() = default;

    // Class Initializers
    void Initialize();
    void Update(float dt);
    void ComputeVertexPoints();
    void CalculateMassForces(float dt);

    void AddSpring(const std::shared_ptr<Spring>& _spring) {
        springs.push_back(_spring);
    }

    // Complex Setters
    void Translate(const Eigen::Vector3f& translation_vector);

    std::vector<Eigen::Vector3f> Colors() const { return colors; }

    auto size() const { return vertices.size(); }

  private:
    // The color of the mass object.
    const Eigen::Vector3f kColor;

    // The original vertex positions
    std::vector<Eigen::Vector3f> original_positions;

    // Represents the colors mapped to each vertex.
    std::vector<Eigen::Vector3f> colors;
};
