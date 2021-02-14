#pragma once

#include "colors.h"
#include "fixture.h"
#include <QDebug>
#include <QMatrix4x4>

constexpr bool kFixedPosition = true;
constexpr bool kUnfixedPosition = false;

class Mass : public Fixture {
  public:
    // The mass of the... mass...
    const float kMass;

    const bool is_fixed;

    Mass()
        : Fixture(QVector4D(0.f, 0.f, 0.f, 0.f), colors::kBlue), kMass(1),
          kSize(1), is_fixed(false) {}
    Mass(bool fixed, QVector4D starting_position)
        : Fixture(starting_position, colors::kBlue), kMass(1), kSize(1),
          is_fixed(fixed) {}
    Mass(float size, float mass, bool fixed, QVector3D color,
         QVector4D starting_position)
        : Fixture(starting_position, color), is_fixed(fixed), kMass(mass),
          kSize(size) {}
    ~Mass() = default;

    void CalculateMassForces(float dt = 0.1f) {
        // We don't want to calculate for fixed masses, that would be bad.
        assert(!is_fixed && "Mass is fixed");
        CalculateAcceleration();

        // Calculate new velocity with respect to time.
        velocity = velocity + acceleration * dt;

        // Calculate new position based on the current velocity with respect to
        // time.
        position = position + velocity * dt;
    }

    inline void Update(float dt) {
        if (is_fixed) {
            return;
        }
        CalculateMassForces(dt);
        ComputeVertexPoints();
    }

    inline void ComputeVertexPoints() {
        // Construct our vertices centered around the origin position supplied
        // on construction
        const auto v1 = QVector4D(position.x() - kSize, position.y() - kSize,
                                  position.z(), 1.f); // Bottom Left
        const auto v2 = QVector4D(position.x() + kSize, position.y() - kSize,
                                  position.z(), 1.f); // Bottom Right
        const auto v3 =
            QVector4D(position.x(), position.y() + kSize, position.z(),
                      1.f); // Top Center

        vertices = std::vector<QVector4D>{{v1, v2, v3}};
    }

    void Translate(const QVector3D& translation_vector) {
        auto translation_matrix = QMatrix4x4();

        translation_matrix.setColumn(3, position);
        translation_matrix.translate(translation_vector);

        position = translation_matrix.column(3);

        // Recompute our vertices
        ComputeVertexPoints();
    }

    /**
        @brief Updates the acceleration of the mass object by some positive or
       negative delta value.
     */
    inline void ChangeAcceleration(const QVector4D& delta) {
        acceleration += delta;
    }

  private:
    // The damping constant to prevent explosiveness
    constexpr static float kDamping = 0.8f;

    // The size of the object centered around the current position.
    const float kSize;

    // Gravitational constant vector, applies -9.81f
    // pounds of negative force
    const QVector4D kGravity = QVector4D(0.0f, -9.81f, 0.0f, 1.0f);

    // The velocity the object is moving at with respect to time.
    QVector4D velocity = QVector4D(0.f, 0.f, 0.f, 0.f);

    // The acceleration of the object with respect to time.
    QVector4D acceleration = QVector4D(0.f, 0.f, 0.f, 0.f);

    void CalculateAcceleration() {
        // Calculate damping dynamically based on how fast the object is moving.
        // Flip the sign of damping since we're adding it to the acceleration
        // value.
        auto damping = (-1 * kDamping * velocity) / kMass;

        // Calculate our acceleration with gravity doing gravity things and
        // damping to prevent explosion.
        acceleration = acceleration + kGravity + damping;
    }
};