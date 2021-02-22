#pragma once

#include <vector>

#include <Eigen/Dense>

/**
 @brief SimObject represents a type of object which can be loaded and interacted
 with in a simulation.
 */
class SimObject {
  public:
    SimObject(Eigen::Vector4f _position, Eigen::Vector3f color)
        : position(_position), kColor(color) {}

    virtual ~SimObject() = default;
    virtual void Initialize() = 0;
    virtual inline void Update(float dt) = 0;
    virtual void Translate(const Eigen::Vector3f& translation_vector) = 0;

    virtual inline void ComputeVertexPoints() = 0;

    std::vector<Eigen::Vector3f> Vertices() { return vertices; }
    std::vector<Eigen::Vector3f> Colors() { return colors; }

    Eigen::Vector4f Position() { return position; }

    auto size() { return vertices.size(); }

  protected:
    // The initialization status of the fixture object.
    bool is_init = false;

    const Eigen::Vector3f kColor;

    // Represents the current position of the fixture.
    Eigen::Vector4f position;

    // Represents the vertices of the fixture.
    std::vector<Eigen::Vector3f> vertices;

    // Represents the colors mapped to each vertex.
    std::vector<Eigen::Vector3f> colors;
};
