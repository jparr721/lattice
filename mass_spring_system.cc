#include "mass_spring_system.h"

#include <iostream>
#include <memory>

MassSpringSystem::MassSpringSystem() {
    initial_conditions = std::make_unique<ShapeSpec>();

    for (int i = 0; i < initial_conditions->sim_objects.size(); ++i) {
        const auto mss = initial_conditions->sim_objects[i];

        const auto mss_masses = mss.masses;
        const auto mss_positions = mss.positions;

        assert(mss_masses.size() == mss_positions.size());

        // Set up all masses with initial positions
        for (int j = 0; j < mss_masses.size(); ++j) {
            const auto mass_node = mss_masses[j];
            auto pos = mss_positions[j];

            const int y = ComputeStartingY(j);
            auto position = Eigen::Vector4f(pos.x(), y, pos.z(), 1.f);
            initial_positions.push_back(position);

            auto mass = std::make_shared<Mass>(mass_node.name, mass_node.fixed,
                                               mass_node.color, position);
            mass->Initialize();
            masses.push_back(std::move(mass));

            // Useful for lookups later
            mass_map.insert(std::pair(mass_node.name, masses.size() - 1));
        }

        for (int j = mss_masses.size() * i;
             j < (mss_masses.size() * i) + mss_masses.size(); ++j) {
            const auto mass_node = mss_masses[j % 8];
            const auto mass = masses[j];
            auto center_node = masses[mass_map.at(mass->Name())];

            for (const auto adjacent_node_name : mass_node.adjacencies) {
                // TODO(@jparr721) - Error handling
                auto adjacent_node = masses[mass_map.at(adjacent_node_name)];

                auto spring = std::make_shared<Spring>(
                    colors::kGreen, adjacent_node, center_node);

                spring->Initialize();
                springs.push_back(std::move(spring));
            }
        }
    }

    std::cout << "Springs: " << springs.size() << std::endl;
    std::cout << "Masses: " << masses.size() << std::endl;

    ComputeShapes();
    ComputeColors();
}

void MassSpringSystem::Reset() {
    const auto last_spring_length = springs[0]->RestLength();
    for (int i = 0; i < masses.size(); ++i) {
        auto mass = masses[i];
        auto position = initial_positions[i];
        position(1) = i < 4 ? last_spring_length : -last_spring_length;

        masses[i]->position = position;
        masses[i]->force = Eigen::Vector4f::Zero();
        masses[i]->velocity = Eigen::Vector4f::Zero();
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
    for (auto spring : springs) {
        spring->CalculateCurrentForce();
    }

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
        for (auto vertex_list : mass->vertices) {
            shapes.push_back(vertex_list);
        }
    }

    for (auto spring : springs) {
        for (auto vertex_list : spring->vertices) {
            shapes.push_back(vertex_list);
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

void MassSpringSystem::SetSpringDampingConstant(float value) {
    for (auto spring : springs) {
        spring->SetDampingConstant(value);
    }
}

Eigen::Vector4f MassSpringSystem::GetFirstMovingMassVelocity() {
    for (auto mass : masses) {
        if (!mass->is_fixed) {
            return mass->velocity;
        }
    }

    return Eigen::Vector4f(0, 0, 0, 0);
}

Eigen::Vector4f MassSpringSystem::GetFirstMovingMassForce() {
    for (auto mass : masses) {
        if (!mass->is_fixed) {
            return mass->force;
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

int MassSpringSystem::ComputeStartingY(int i) {
    return i < 4 ? Spring::kMinimumSpringRestLengthValue
                 : -Spring::kMinimumSpringRestLengthValue;
}
