//
//  spring.cpp
//  lattice
//
//  Created by Jarred Parr on 2/6/21.
//

#include "spring.hpp"

void Spring::ComputeVertexPoints() {
    // Construct vertices around our origin position
    const auto v1 = glm::vec4(left_mass->Position().x, left_mass->Position().y, left_mass->Position().z, 1.f); // Bottom Left
    const auto v2 = glm::vec4(right_mass->Position().x, right_mass->Position().y, right_mass->Position().z, 1.f); // Bottom Left

    vertices = std::vector<glm::vec4>{{v1, v2}};
}

void Spring::ComputeShapeWithColor() {
    // Pre allocate the vector with zero floats
    shape = std::vector<GLfloat>(24, 0.f);
    
    for (int i = 0; i < vertices.size(); ++i) {
        const auto vertex = vertices[i];

        int j = i * 6;

        // Insert Vertex Positions
        shape[j] = vertex.x;
        shape[j + 1] = vertex.y;
        shape[j + 2] = vertex.z;

        // Insert Colors
        shape[j + 3] = kColor.r;
        shape[j + 4] = kColor.g;
        shape[j + 5] = kColor.b;
    }
}

/**
    @brief Computes the distance between the two internal mass vectors.
 */
float Spring::ComputeVectorDistance() {
    const float x_distance = std::pow(left_mass->Position().x - right_mass->Position().x, 2);
    const float y_distance = std::pow(left_mass->Position().y - right_mass->Position().y, 2);
    const float z_distance = std::pow(left_mass->Position().z - right_mass->Position().z, 2);
    
    return std::sqrt(x_distance + y_distance + z_distance);
}

void Spring::ApplySpringForces() {
    const float current_spring_length = ComputeVectorDistance();
    
    // Calculate the vector norm so we can express hooke's law as a directional vector
    const glm::vec4 mass_norm = (left_mass->Position() - right_mass->Position()) / current_spring_length;
    
    const glm::vec3 force = kStiffness * (current_spring_length - kLength) * mass_norm;
    const glm::vec4 mass_acceleration_delta = glm::vec4(force / right_mass->kMass, 1.f);
    
    // Update acceleration for the right mass. Right now, this is all that's needed, for later
    // sims, we will need to change this to support multi-mass updates (which should be easy).
    // We are only updating the right mass because the left mass is a fixed-point anchor for the
    // spring to hook to.
    right_mass->ChangeAcceleration(mass_acceleration_delta);
}
