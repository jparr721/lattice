#include "mass_spring_system.h"

void MassSpringSystem::Initialize() {
    assert(size() > 0 && "Must add fixtures for simulation");

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

void MassSpringSystem::Update(float dt) {
    for (auto mass : masses) {
        mass->Update(dt);
    }

    for (auto spring : springs) {
        spring->Update(dt);
    }
    ComputeVertexPoints();
    ComputeShapes();
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