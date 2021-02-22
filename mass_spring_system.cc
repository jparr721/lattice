#include "mass_spring_system.h"
#include "shape_spec.h"

#include <iostream>
#include <memory>

void MassSpringSystem::Initialize() {
    auto shape_spec = std::make_unique<ShapeSpec>();

    const float top_x = -0.5f;
    const float top_y = 0.5f;
    const float top_z = 0.0f;

    const float bottom_x = -0.5f;
    const float bottom_y = -0.5f;
    const float bottom_z = 0.0f;

    assert(shape_spec->graph.size() % 2 == 0);

    int i = 0;
    for (const auto& [node, adjacencies] : shape_spec->graph) {
        // Top Masses
        if (i < 4) {
            auto position =
                Eigen::Vector4f(top_x + (i * .4), top_y, top_z, 1.0f);
            auto mass = std::make_shared<Mass>(node.size, node.name, node.fixed,
                                               node.color, position);
            AddMass(mass);
            top_masses.push_back(mass);
        } else { // Bottom Masses
            auto position = Eigen::Vector4f(bottom_x + ((i % 4) * .4), bottom_y,
                                            bottom_z, 1.0f);
            auto mass = std::make_shared<Mass>(node.size, node.name, node.fixed,
                                               node.color, position);
            AddMass(mass);
            bottom_masses.push_back(mass);
        }
        ++i;
    }

    for (const auto& [node, adjacencies] : shape_spec->graph) {
        if (adjacencies.size() > 0) {
            // This'll break when we add more complex shapes!
            const MassNode left_mass_node = adjacencies[0];
            const MassNode right_mass_node = adjacencies[1];

            auto center_node = GetMassByName(node.name);
            auto left_adjacent_node = GetMassByName(left_mass_node.name);
            auto right_adjacent_node = GetMassByName(right_mass_node.name);

            assert(center_node != std::nullopt);
            assert(left_adjacent_node != std::nullopt);
            assert(right_adjacent_node != std::nullopt);

            auto left_spring = std::make_shared<Spring>(
                1.0f, 0.5f, colors::kGreen, left_adjacent_node.value(),
                center_node.value());

            auto right_spring = std::make_shared<Spring>(
                1.0f, 0.5f, colors::kGreen, center_node.value(),
                right_adjacent_node.value());

            AddSpring(left_spring);
            AddSpring(right_spring);
        }
    }

    for (auto mass : masses) {
        mass->Initialize();
    }

    for (auto spring : springs) {
        spring->Initialize();
    }

    ComputeVertexPoints();
    ComputeShapes();
    ComputeColors();

    is_init = true;
}

void MassSpringSystem::Redraw() {
    ComputeVertexPoints();
    ComputeShapes();
}

void MassSpringSystem::Update(float dt) {
    for (auto mass : masses) {
        mass->Update(dt);
    }

    for (auto spring : springs) {
        spring->Update(dt);
    }

    Redraw();
}

void MassSpringSystem::AddSpring(const std::shared_ptr<Spring>& spring) {
    assert(!is_init && "Cannot change an initialized system");
    springs.push_back(std::move(spring));
}

void MassSpringSystem::AddMass(const std::shared_ptr<Mass>& mass) {
    assert(!is_init && "Cannot change an initialized system");
    masses.push_back(std::move(mass));
}

void MassSpringSystem::ComputeColors() {
    for (auto mass : masses) {
        for (auto _colors : mass->Colors()) {
            colors.push_back(_colors);
        }
    }

    for (auto spring : springs) {
        for (auto _colors : spring->Colors()) {
            colors.push_back(_colors);
        }
    }
}

void MassSpringSystem::ComputeVertexPoints() {
    for (auto mass : masses) {
        // TODO(@jparr721) - Collision detection
        mass->ComputeVertexPoints();
    }

    for (auto spring : springs) {
        // TODO(@jparr721) - Collision detection
        spring->ComputeVertexPoints();
    }
}

void MassSpringSystem::ComputeShapes() {
    // Get the shape size for re-initialization.
    auto shapes_size = shapes.size();

    // First, clear the existing shape.
    shapes.clear();

    // Then, reserve the space again.
    if (shapes_size > 0)
        shapes.reserve(shapes_size);

    for (auto mass : masses) {
        for (auto vertices : mass->Vertices()) {
            shapes.push_back(vertices);
        }
    }

    for (auto spring : springs) {
        for (auto vertices : spring->Vertices()) {
            shapes.push_back(vertices);
        }
    }
}

void MassSpringSystem::SetSpringStiffness(float value) {
    for (auto spring : springs) {
        spring->SetStiffness(value);
    }
}

void MassSpringSystem::SetSpringRestLength(float value) {
    for (auto spring : springs) {
        spring->SetRestLength(value);
    }
}

void MassSpringSystem::SetMassWeight(float value) {
    for (auto mass : masses) {
        mass->SetWeight(value);
    }
}

void MassSpringSystem::SetMassDampingConstant(float value) {
    for (auto mass : masses) {
        mass->SetDampingConstant(value);
    }
}

Eigen::Vector4f MassSpringSystem::GetFirstMovingMassVelocity() {
    for (auto mass : masses) {
        if (!mass->is_fixed) {
            return mass->Velocity();
        }
    }

    return Eigen::Vector4f(0, 0, 0, 0);
}

Eigen::Vector4f MassSpringSystem::GetFirstMovingMassAcceleration() {
    for (auto mass : masses) {
        if (!mass->is_fixed) {
            return mass->Acceleration();
        }
    }

    return Eigen::Vector4f(0, 0, 0, 0);
}

Eigen::Vector4f MassSpringSystem::GetFirstSpringForce() {
    assert(is_init && "Must be initialized");
    assert(springs.size() > 0);
    const auto spring = springs[0];

    return spring->Force();
}

std::optional<std::shared_ptr<Mass>>
MassSpringSystem::GetMassByName(const std::string& name) {
    for (auto mass : masses) {
        if (mass->Name() == name) {
            return mass;
        }
    }

    return std::nullopt;
}

// TODO(@jparr721) - THIS IS BAD CODE DELETE LATER
void MassSpringSystem::TranslateTopGroup(const Eigen::Vector3f& direction) {
    for (auto mass : top_masses) {
        // Translate each individual mass
        mass->Translate(direction);
    }
}

// TODO(@jparr721) - THIS IS BAD CODE DELETE LATER
void MassSpringSystem::TranslateBottomGroup(const Eigen::Vector3f& direction) {
    for (auto mass : bottom_masses) {
        // Translate each individual mass
        mass->Translate(direction);
    }
}
