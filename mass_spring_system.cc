#include "mass_spring_system.h"

void MassSpringSystem::Initialize() {
    assert(size() > 0 && "Must add fixtures for simulation");

    for (auto fixture : fixtures) {
        fixture->Initialize();
    }

    ComputeVertexPoints();
    ComputeShapes();
    ComputeColors();

    is_init = true;
}

void MassSpringSystem::Update(float dt) {
    for (auto fixture : fixtures) {
        fixture->Update(dt);
    }
    ComputeVertexPoints();
    ComputeShapes();
}

void MassSpringSystem::AddFixture(const std::shared_ptr<Fixture>& fixture) {
    assert(!is_init && "Cannot change an initialized system");
    fixtures.push_back(std::move(fixture));
}

void MassSpringSystem::ComputeColors() {
    for (auto fixture : fixtures) {
        for (auto _colors : fixture->Colors()) {
            colors.push_back(_colors);
        }
    }
}

void MassSpringSystem::ComputeVertexPoints() {
    for (auto fixture : fixtures) {
        // TODO(@jparr721) - Collision detection
        fixture->ComputeVertexPoints();
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

    for (auto fixture : fixtures) {
        for (auto vertices : fixture->Vertices()) {
            // Append all of the vertices to the shape object.
            shapes.push_back(vertices);
        }
    }
}