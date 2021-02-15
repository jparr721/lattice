#pragma once

#include "colors.h"
#include "fixture.h"
#include "mass.h"
#include <memory>

class Spring : public Fixture {
  public:
    Spring(float stiffness, float resting_length, QVector3D color,
           std::shared_ptr<Mass>& _left_mass,
           std::shared_ptr<Mass>& _right_mass)
        : Fixture(QVector4D(0.f, 0.f, 0.f, 1.f), colors::kGreen),
          kStiffness(stiffness), kLength(resting_length), left_mass(_left_mass),
          right_mass(_right_mass) {}

    void Initialize() {
        ComputeVertexPoints();

        colors = std::vector<QVector3D>{{kColor, kColor, kColor}};

        is_init = true;
    }

    inline void Update(float dt) { ApplySpringForces(); }

    inline void ComputeVertexPoints() {
        const auto lpos = left_mass->Position();
        const auto rpos = right_mass->Position();

        const auto v1 = QVector3D(lpos.x(), lpos.y(), position.z());
        const auto v2 = QVector3D(rpos.x(), rpos.y(), position.z());
        const auto v3 = QVector3D(rpos.x() + 0.1f, rpos.y(), position.z());

        vertices = std::vector<QVector3D>{{v1, v2, v3}};
    }

    void Translate(const QVector3D& translation_vector) { return; }

  private:
    // The spring k value.
    const float kStiffness;

    // The resting length of the spring in the Y direction.
    const float kLength;

    std::shared_ptr<Mass> left_mass;
    std::shared_ptr<Mass> right_mass;

    void ApplySpringForces() {
        const auto current_spring_length =
            left_mass->Position().toVector3D().distanceToPoint(
                right_mass->Position().toVector3D());

        const auto lr_diff = left_mass->Position() - right_mass->Position();
        auto mass_norm = lr_diff.normalized();

        auto force = kStiffness * (current_spring_length - kLength) * mass_norm;
        QVector4D mass_acceleration_delta = force / right_mass->kMass;

        right_mass->ChangeAcceleration(mass_acceleration_delta);
    }
};