#include "mass_spring_system.h"
#include "shape_spec.h"

#include <iostream>
#include <memory>

MassSpringSystem::MassSpringSystem() {
    auto shape_spec = std::make_unique<ShapeSpec>();

    assert(shape_spec->graph.size() % 2 == 0);

    int i = 0;
    for (const auto& [node, adjacencies] : shape_spec->graph) {
        const auto xy = ComputeStartingPosition(i);
        const auto x = std::get<0>(xy);
        const auto y = std::get<1>(xy);

        auto position = Eigen::Vector4f(x, y, 0.0f, 1.0f);
        auto mass =
            std::make_shared<Mass>(node.size, kMinimumMassValue, node.name,
                                   node.fixed, node.color, position);
        mass->Initialize();
        masses.push_back(std::move(mass));

        if (i < 4) {
            top_masses.push_back(mass);
        } else {
            bottom_masses.push_back(mass);
        }

        ++i;
    }


    for (const auto& [node, adjacencies] : shape_spec->graph) {
        auto center_node = GetMassByName(node.name);
        assert(center_node != std::nullopt);

        for (auto adjacent_mass_node : adjacencies) {
            auto adjacent_node = GetMassByName(adjacent_mass_node.name);
            assert(adjacent_node != std::nullopt);

            auto spring = std::make_shared<Spring>(
                kMinimumSpringConstantValue, kMinimumSpringRestLengthValue,
                colors::kGreen, adjacent_node.value(), center_node.value());
            spring->Initialize();
            springs.push_back(std::move(spring));
        }
    }

    ComputeShapes();
    ComputeColors();
}

void MassSpringSystem::Reset() {
    for (int i = 0; i < masses.size(); ++i) {
        const auto xy = ComputeStartingPosition(i);
        const auto x = std::get<0>(xy);
        const auto y = std::get<1>(xy);
        const auto position = Eigen::Vector4f(x, y, 0.0f, 1.0f);

        const auto zero_vector = Eigen::Vector4f(0.f, 0.f, 0.f, 0.f);

        masses[i]->SetPosition(position);
        masses[i]->SetAcceleration(zero_vector);
        masses[i]->SetVelocity(zero_vector);
    }

    for (auto mass : masses) {
        mass->ComputeVertexPoints();
    }

    Redraw();
}

void MassSpringSystem::Redraw() {
    for (auto spring : springs) {
        spring->ComputeVertexPoints();
    }

    ComputeShapes();
}

void MassSpringSystem::Update() {
    for (auto mass : masses) {
        mass->Update(timestep_size);
    }

    Redraw();
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

std::pair<int, int> MassSpringSystem::ComputeStartingPosition(int i) {
    return std::pair<int, int>((i % 4), i < 4 ? 1 : -1);
}
