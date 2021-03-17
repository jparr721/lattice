#include <lattice/mass_spring_system.h>

#include <memory>

MassSpringSystem::MassSpringSystem(const MSSConfig& config) {
    name = config.name;
    const auto mss_masses = config.masses;
    const auto mss_positions = config.positions;

    assert(mss_masses.size() == mss_positions.size());

    // Set up all masses with initial positions
    for (int j = 0; j < mss_masses.size(); ++j) {
        const auto mass_node = mss_masses[j];
        const auto pos = mss_positions[j];

        const int y = mass_node.fixed ? -Spring::kMinimumSpringRestLengthValue
                                      : Spring::kMinimumSpringRestLengthValue;

        const auto position = Eigen::Vector3f(pos.x(), y, pos.z());

        initial_positions.push_back(position);

        const auto mass = std::make_shared<Mass>(
            mass_node.number, mass_node.fixed, mass_node.color, position);
        mass->Initialize();
        masses.push_back(mass);
        mass_map.insert(std::pair(mass_node.number, masses.size() - 1));
    }

    for (int j = 0; j < mss_masses.size(); ++j) {
        const auto mass_node = mss_masses[j];
        const auto mass = masses[j];
        auto center_node = masses[mass_map.at(mass->number)];

        for (const auto adjacent_node_number : mass_node.adjacencies) {
            auto adjacent_node = masses[mass_map.at(adjacent_node_number)];

            auto spring = std::make_shared<Spring>(Colors::kGreen, center_node,
                                                   adjacent_node);

            spring->Initialize();
            springs.push_back(std::move(spring));
        }
    }

    PreloadModelData();
    ComputeShapes();
    ComputeColors();
}

void MassSpringSystem::Reset() {
    const auto last_spring_length = springs[0]->RestLength();
    for (int i = 0; i < masses.size(); ++i) {
        auto mass = masses[i];
        auto position = initial_positions[i];
        const int y = mass->fixed ? -last_spring_length : last_spring_length;
        position(1) = y;

        masses[i]->position = position;
        masses[i]->force = Eigen::Vector3f::Zero();
        masses[i]->velocity = Eigen::Vector3f::Zero();
        masses[i]->rest_position = position;
    }

    for (const auto& mass : masses) {
        mass->ComputeVertexPoints();
    }

    Redraw();
}

void MassSpringSystem::Redraw() {
    for (const auto& spring : springs) {
        spring->ComputeVertexPoints();
    }

    ComputeShapes();
}

void MassSpringSystem::Update(float dt) {
    for (const auto& spring : springs) {
        spring->CalculateCurrentForce();
    }

    for (const auto& mass : masses) {
        mass->Update(dt);
    }

    Redraw();
}

void MassSpringSystem::ComputeColors() {
    for (const auto& mass : masses) {
        for (const auto& _colors : mass->Colors()) {
            colors.push_back(_colors);
        }
    }

    for (const auto& spring : springs) {
        for (const auto& _colors : spring->Colors()) {
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

    for (const auto& mass : masses) {
        for (const auto& vertex_list : mass->vertices) {
            shapes.push_back(vertex_list);
        }
    }

    for (const auto& spring : springs) {
        for (const auto& vertex_list : spring->vertices) {
            shapes.push_back(vertex_list);
        }
    }
}

void MassSpringSystem::SetSpringConstant(float value) {
    for (const auto& spring : springs) {
        spring->SetStiffness(value);
    }
}

void MassSpringSystem::SetSpringRestLength(float value) {
    for (const auto& spring : springs) {
        spring->SetRestLength(value);
    }
}

void MassSpringSystem::SetMassWeight(float value) {
    for (const auto& mass : masses) {
        mass->SetWeight(value);
    }
}

void MassSpringSystem::SetSpringDampingConstant(float value) {
    for (const auto& spring : springs) {
        spring->SetDampingConstant(value);
    }
}

std::unordered_map<int, Eigen::Vector3f> MassSpringSystem::GetMassVelocities() {
    for (const auto& mass : masses) {
        mass_velocities[mass->number] = mass->velocity;
    }

    return mass_velocities;
}

std::unordered_map<int, Eigen::Vector3f> MassSpringSystem::GetMassForces() {
    for (const auto& mass : masses) {
        mass_forces[mass->number] = mass->force;
    }

    return mass_forces;
}

void MassSpringSystem::PreloadModelData() {
    for (const auto& mass : masses) {
        mass_forces.insert(std::pair(mass->number, Eigen::Vector3f::Zero()));

        mass_velocities.insert(
            std::pair(mass->number, Eigen::Vector3f::Zero()));
    }
}