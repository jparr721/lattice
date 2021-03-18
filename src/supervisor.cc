#include <lattice/supervisor.h>

Supervisor::Supervisor(const MSSConfig& config) {
    simulations.push_back(std::make_shared<MassSpringSystem>(config));

    for (const auto& simulation : simulations) {
        for (const auto& color : simulation->Colors()) {
            colors.push_back(color);
        }
    }

    shapes =
        std::vector<Eigen::Vector3f>(colors.size(), Eigen::Vector3f::Zero());
    RecompileVertexBuffer();
}

void Supervisor::RecompileVertexBuffer() {
    shapes.clear();
    for (const auto& simulation : simulations) {
        for (const auto& shape : simulation->Shapes()) {
            shapes.push_back(shape);
        }
    }
}

void Supervisor::Update() {
    for (const auto& simulation : simulations) {
        simulation->Update(timestep_size);
    }

    RecompileVertexBuffer();
}

void Supervisor::Reset() {
    for (const auto& simulation : simulations) {
        simulation->Reset();
    }

    RecompileVertexBuffer();
}

void Supervisor::SetMassWeight(float value) {
    for (const auto& simulation : simulations) {
        simulation->SetMassWeight(value);
    }
}

void Supervisor::SetSpringConstant(float value) {
    for (const auto& simulation : simulations) {
        simulation->SetSpringConstant(value);
    }
}

void Supervisor::SetSpringDampingConstant(float value) {
    for (const auto& simulation : simulations) {
        simulation->SetSpringDampingConstant(value);
    }
}

void Supervisor::SetSpringRestLength(float value) {
    for (const auto& simulation : simulations) {
        simulation->SetSpringRestLength(value);
    }
}

std::unordered_map<std::string, std::unordered_map<int, Eigen::Vector3f>>
Supervisor::SampleMassVelocities() {
    for (const auto& simulation : simulations) {
        const auto velocities = simulation->GetMassVelocities();
        current_mass_velocities[simulation->name] = velocities;
    }

    return current_mass_velocities;
}

std::unordered_map<std::string, std::unordered_map<int, Eigen::Vector3f>>
Supervisor::SampleMassForces() {
    for (const auto& simulation : simulations) {
        const auto forces = simulation->GetMassForces();
        current_mass_forces[simulation->name] = forces;
    }

    return current_mass_forces;
}
