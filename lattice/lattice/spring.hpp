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
    Spring(float stiffness, float resting_length,
           std::shared_ptr<Mass>& _left_mass,
           std::shared_ptr<Mass>& _right_mass)
        : Fixture(glm::vec4(1.f, 1.f, 1.f, 0.f), colors::kRed),
          kStiffness(stiffness), kLength(resting_length), left_mass(_left_mass),
          right_mass(_right_mass) {}

    inline void Update(float dt) { return; }

    inline void ComputeVertexPoints() {
        const auto v1 =
            glm::vec4(left_mass->Position().x, left_mass->Position().y,
                      left_mass->Position().z, 1.f);
        const auto v2 =
            glm::vec4(right_mass->Position().x, right_mass->Position().y,
                      right_mass->Position().z, 1.f);

        vertices = std::vector<glm::vec4>{{v1, v2}};
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

    // The position of our rendered spring.
    glm::vec4 position;

    void ApplySpringForces();
    // TODO(@jparr721) - Make this a helper.
    float ComputeVectorDistance();
};

#endif /* spring_hpp */
