#pragma once

#include <QVector3D>
#include <QVector4D>
#include <vector>

/**
 @brief Fixture represents a type of object which can be loaded and interacted
 with in a simulation.
 */
class Fixture {
  public:
    Fixture(QVector4D _position, QVector3D color)
        : position(_position), kColor(color) {}

    virtual ~Fixture() = default;
    virtual inline void Update(float dt) = 0;
    virtual void Translate(const QVector3D& translation_vector) = 0;

    virtual inline void ComputeVertexPoints() = 0;

    std::vector<QVector4D> Vertices() { return vertices; }

    QVector3D Color() { return kColor; }
    QVector4D Position() { return position; }

    auto size() { return vertices.size(); }

  protected:
    const QVector3D kColor;

    // Represents the current position of the fixture.
    QVector4D position;

    // Represents the vertices of the fixture.
    std::vector<QVector4D> vertices;
};