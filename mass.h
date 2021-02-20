#pragma once

#include "colors.h"
#include "sim_object.h"

#include <string>

#include <QDebug>
#include <QMatrix4x4>

constexpr bool kFixedPosition = true;
constexpr bool kUnfixedPosition = false;

class Mass : public SimObject {
  public:
    const bool is_fixed;

    Mass()
        : SimObject(QVector4D(0.f, 0.f, 0.f, 0.f), colors::kBlue),
          mass_weight(1), mass_size(1), is_fixed(false) {}
    Mass(bool fixed, QVector4D starting_position)
        : SimObject(starting_position, colors::kBlue), mass_weight(1),
          mass_size(1), is_fixed(fixed) {}
    Mass(float size, std::string name, bool fixed, QVector3D color,
         QVector4D starting_position)
        : SimObject(starting_position, color), is_fixed(fixed), name(name),
          mass_size(size) {}
    ~Mass() = default;

    std::string Name() { return name; }

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

    void Initialize() {
        ComputeVertexPoints();

        colors = std::vector<QVector3D>{{kColor, kColor, kColor}};

        is_init = true;
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
        const auto v1 =
            QVector3D(position.x() - mass_size, position.y() - mass_size,
                      position.z()); // Bottom Left
        const auto v2 =
            QVector3D(position.x() + mass_size, position.y() - mass_size,
                      position.z()); // Bottom Right
        const auto v3 = QVector3D(position.x(), position.y() + mass_size,
                                  position.z()); // Top Center

        vertices = std::vector<QVector3D>{{v1, v2, v3}};
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

    void SetWeight(float value) { mass_weight = value; }
    void SetDampingConstant(float value) { damping_constant = value; }

    float Weight() { return mass_weight; }
    float DampingConstant() { return damping_constant; }

    QVector4D Velocity() const { return velocity; }
    QVector4D Acceleration() const { return acceleration; }

  private:
    // The name of the mass node
    std::string name;

    // The mass of the... mass...
    float mass_weight = 0.5f;

    // The damping constant to prevent explosiveness
    float damping_constant = 0.8f;

    // The size of the object centered around the current position.
    float mass_size;

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
        auto damping = (-1 * damping_constant * velocity) / mass_weight;

        // Calculate our acceleration with gravity doing gravity things and
        // damping to prevent explosion.
        acceleration = acceleration + kGravity + damping;
    }
};
