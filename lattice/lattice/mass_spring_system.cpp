//
//  shape.cpp
//  lattice
//
//  Created by Jarred Parr on 2/7/21.
//

#include "mass_spring_system.hpp"

void MassSpringSystem::MoveFixtureAtIndex(int index,
                                          const glm::vec3& translation_vector) {
    assert(index < fixtures.size());
    fixtures[index]->Translate(translation_vector);

    // Inefficient, I need to change this later to recompute only changed
    // shapes.
    ComputeVertexPoints();
    ComputeShapes();
}
