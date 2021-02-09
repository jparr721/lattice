//
//  spring.hpp
//  lattice
//
//  Created by Jarred Parr on 2/6/21.
//

#ifndef spring_hpp
#define spring_hpp

#include "colors.hpp"
#include "fixture.hpp"
#include "mass.hpp"
#include <memory>

class Spring : public Fixture {
  public:
    Spring(float stiffness, float resting_length, glm::vec4 starting_position,
           std::shared_ptr<Mass>& _left_mass,
           std::shared_ptr<Mass>& _right_mass)
        : Fixture(starting_position, colors::kGreen), kStiffness(stiffness),
          kLength(resting_length), left_mass(_left_mass),
          right_mass(_right_mass) {}

    inline void Update(float dt) { return; }

    inline void ComputeVertexPoints() {
        const auto lpos = left_mass->Position();
        const auto rpos = right_mass->Position();

        const auto v1 = glm::vec4(lpos.x, lpos.y, position.z,
                                  1.f); // Bottom Left
        const auto v2 = glm::vec4(rpos.x, rpos.y, position.z,
                                  1.f); // Bottom Right
        const auto v3 = glm::vec4(rpos.x + 0.1f, rpos.y, position.z,
                                  1.f); // Top Center

        vertices = std::vector<glm::vec4>{{v1, v2, v3}};
    }

    void Translate(const glm::vec3& translation_vector) {
        auto transformation_matrix =
            glm::translate(glm::mat4(1.0f), translation_vector);

        // Apply a stretch operation to the vertices
        // TODO

        // Recompute the vertices
        ComputeVertexPoints();
    }

  private:
    // The spring stiffness.
    const float kStiffness;

    // The resting length of the spring in the Y direction.
    const float kLength;

    std::shared_ptr<Mass> left_mass;
    std::shared_ptr<Mass> right_mass;

    void ApplySpringForces();
    // TODO(@jparr721) - Make this a helper.
    float ComputeVectorDistance();
};

#endif /* spring_hpp */
