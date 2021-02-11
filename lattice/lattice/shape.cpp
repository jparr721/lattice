//
//  shape.cpp
//  lattice
//
//  Created by Jarred Parr on 2/7/21.
//

#include "shape.hpp"

void Shape::MoveFixtureAtIndex(int index, const glm::vec3& translation_vector) {
    assert(index < fixtures.size());
    fixtures[index]->Translate(translation_vector);
    ComputeVertexPoints();
    ComputeShapes();
}
