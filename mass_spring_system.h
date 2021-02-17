#pragma once

#include <QVector3D>
#include <vector>

#include "fixture.h"
#include "plotter.h"
#include "spring.h"
#include "mass.h"

class MassSpringSystem {
  public:
    MassSpringSystem() = default;
    ~MassSpringSystem() = default;

    void Initialize();
    void Update(float dt = 0.1);

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

    std::vector<QVector3D> Colors() { return colors; }
    std::vector<QVector3D> Shapes() { return shapes; }

    auto size() { return springs.size() + masses.size(); }

  private:
    bool is_init = false;

    // The springs in the sim
    std::vector<std::shared_ptr<Spring>> springs;

    // The masses in the sim
    std::vector<std::shared_ptr<Mass>> masses;

    // Our constructed shapes in a flat list.
    std::vector<QVector3D> shapes;

    // Our shapes' colors in a flat list.
    std::vector<QVector3D> colors;
};