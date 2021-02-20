#pragma once

#include "mass.h"
#include "spring.h"

#include <optional>
#include <vector>

#include <QVector3D>

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

    // Mass Getter
    std::optional<std::shared_ptr<Mass>> GetMassByName(const std::string& name);

    // Mass Plottable Getters
    QVector4D GetFirstMovingMassVelocity();
    QVector4D GetFirstMovingMassAcceleration();

    // Spring Plottable Getters
    QVector4D GetFirstSpringForce();

    std::vector<QVector3D> Colors() { return colors; }
    std::vector<QVector3D> Shapes() { return shapes; }

    auto size() { return springs.size() + masses.size(); }

    // TODO(@jparr721) - Change when refactoring later!!!
    void TranslateTopGroup(const QVector3D& direction);
    void TranslateBottomGroup(const QVector3D& direction);

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


    // TODO(@jparr721) - This is not a good way to do this which scales well.
    std::vector<std::shared_ptr<Mass>> top_masses;
    std::vector<std::shared_ptr<Mass>> bottom_masses;

    void Redraw();
};
