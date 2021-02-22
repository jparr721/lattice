#pragma once

#include "colors.h"
#include "utility.h"

#include <string>

#include <QDebug>

class Mass {
  public:
    const bool is_fixed;

    Mass()
        : position(Eigen::Vector4f(0.f, 0.f, 0.f, 0.f)), kColor(colors::kBlue),
          mass_weight(1), mass_size(1), is_fixed(false) {}
    Mass(float size, std::string name, bool fixed, Eigen::Vector3f color,
         Eigen::Vector4f starting_position)
        : position(starting_position), kColor(color), is_fixed(fixed),
          name(name), mass_size(size) {}
    ~Mass() = default;

    // Class Initializers
    void Initialize();
    void Update(float dt);
    void ComputeVertexPoints();
    void CalculateMassForces(float dt);

    // Trivial Setters
    void SetWeight(float value) { mass_weight = value; }
    void SetDampingConstant(float value) { damping_constant = value; }

    // Complex Setters
    /**
        @brief Updates the acceleration of the mass object by some positive or
       negative delta value.
     */
    void ChangeAcceleration(const Eigen::Vector4f& delta);
    void Translate(const Eigen::Vector3f& translation_vector);

    // Trivial Getters
    float Weight() const { return mass_weight; }
    float DampingConstant() const { return damping_constant; }

    std::string Name() { return name; }

    Eigen::Vector4f Velocity() const { return velocity; }
    Eigen::Vector4f Acceleration() const { return acceleration; }
    Eigen::Vector4f Position() const { return position; }

    std::vector<Eigen::Vector3f> Vertices() const { return vertices; }
    std::vector<Eigen::Vector3f> Colors() const { return colors; }

    auto size() const { return vertices.size(); }

  private:
    // The initialization status of the fixture object.
    bool is_init = false;

    // Gravitational constant vector, applies -9.81f
    // pounds of negative force
    const Eigen::Vector4f kGravity = Eigen::Vector4f(0.0f, -9.81f, 0.0f, 1.0f);

    // The color of the mass object.
    const Eigen::Vector3f kColor;

    // The name of the mass node
    std::string name;

    // The mass of the... mass...
    float mass_weight = 0.5f;

    // The damping constant to prevent explosiveness
    float damping_constant = 0.8f;

    // The size of the object centered around the current position.
    float mass_size;

    // Represents the current position of the fixture.
    Eigen::Vector4f position;

    // Represents the vertices of the fixture.
    std::vector<Eigen::Vector3f> vertices;

    // Represents the colors mapped to each vertex.
    std::vector<Eigen::Vector3f> colors;

    // The velocity the object is moving at with respect to time.
    Eigen::Vector4f velocity = Eigen::Vector4f(0.f, 0.f, 0.f, 0.f);

    // The acceleration of the object with respect to time.
    Eigen::Vector4f acceleration = Eigen::Vector4f(0.f, 0.f, 0.f, 0.f);

    void CalculateAcceleration();
};
