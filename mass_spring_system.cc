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

            const int y = ComputeY(j, mss_masses.size(),
                                   Spring::kMinimumSpringRestLengthValue);

            auto position = Eigen::Vector3f(pos.x(), y, pos.z());

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
            const auto mass_node = mss_masses[j % mss_masses.size()];
            const auto mass = masses[j];
            auto center_node = masses[mass_map.at(mass->Name())];

            for (const auto adjacent_node_name : mass_node.adjacencies) {
                // TODO(@jparr721) - Error handling
                auto adjacent_node = masses[mass_map.at(adjacent_node_name)];

                auto spring = std::make_shared<Spring>(
                    colors::kGreen, center_node, adjacent_node);

                spring->Initialize();
                springs.push_back(std::move(spring));
            }
        }
    }

    ComputeShapes();
    ComputeColors();
}

void MassSpringSystem::Reset() {
    const auto last_spring_length = springs[0]->RestLength();
    for (int i = 0; i < masses.size(); ++i) {
        auto mass = masses[i];
        auto position = initial_positions[i];
        const int y =
            ComputeY(i, initial_conditions->sim_objects[0].masses.size(),
                     last_spring_length);
        position(1) = y;

        masses[i]->position = position;
        masses[i]->force = Eigen::Vector3f::Zero();
        masses[i]->velocity = Eigen::Vector3f::Zero();
        masses[i]->rest_position = position;
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

Eigen::Vector3f MassSpringSystem::GetFirstMovingMassVelocity() {
    for (auto mass : masses) {
        if (!mass->is_fixed) {
            return mass->velocity;
        }
    }

    return Eigen::Vector3f(0, 0, 0);
}

Eigen::Vector3f MassSpringSystem::GetFirstMovingMassForce() {
    for (auto mass : masses) {
        if (!mass->is_fixed) {
            return mass->force;
        }
    }

    return Eigen::Vector3f(0, 0, 0);
}

Eigen::Vector3f MassSpringSystem::GetFirstSpringForce() {
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

int MassSpringSystem::ComputeY(int index, int total_masses, int rest_length) {
    const int split_point = total_masses / 2;

    return (index % total_masses) < split_point ? rest_length : -rest_length;
}
