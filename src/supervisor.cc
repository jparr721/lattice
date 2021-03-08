#include <lattice/supervisor.h>

#include <iostream>

Supervisor::Supervisor(std::shared_ptr<ShapeSpec>& shape_spec) {
    for (const auto& initial_conditions : shape_spec->sim_objects) {
        simulations.push_back(
            std::make_shared<MassSpringSystem>(initial_conditions));
    }

    for (const auto simulation : simulations) {
        for (const auto color : simulation->Colors()) {
            colors.push_back(color);
        }
    }
    
    shapes = std::vector<Eigen::Vector3f>(colors.size(), Eigen::Vector3f::Zero());
    RecompileVertexBuffer();
}

void Supervisor::RecompileVertexBuffer() {
    shapes.clear();
    for (const auto simulation : simulations) {
        for (const auto shape : simulation->Shapes()) {
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

Eigen::Vector3f Supervisor::SampleMassVelocity() {}

Eigen::Vector3f Supervisor::SampleMassForce() {}
