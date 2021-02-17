#pragma once

#include <QVector3D>
#include <vector>

#include "fixture.h"
#include "plotter.h"

class MassSpringSystem {
  public:
    MassSpringSystem() = default;
    ~MassSpringSystem() = default;

    void Initialize();
    void Update(float dt = 0.1);

    void AddFixture(const std::shared_ptr<Fixture>& fixture);

    void ComputeVertexPoints();
    void ComputeShapes();
    void ComputeColors();

    std::vector<QVector3D> Colors() { return colors; }
    std::vector<QVector3D> Shapes() { return shapes; }

    auto size() { return fixtures.size(); }

  private:
    bool is_init = false;
    // Our list of fixtures to work with.
    std::vector<std::shared_ptr<Fixture>> fixtures;

    // Our constructed shapes in a flat list.
    std::vector<QVector3D> shapes;

    // Our shapes' colors in a flat list.
    std::vector<QVector3D> colors;
};