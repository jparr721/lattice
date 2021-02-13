//
//  fixture.hpp
//  lattice
//
//  Created by Jarred Parr on 2/7/21.
//

#ifndef fixture_hpp
#define fixture_hpp

#include <vector>

#include <glm/glm.hpp>

/**
 @brief Fixture represents a type of object which can be loaded and interacted
 with in a simulation.
 */
class Fixture {
  public:
    Fixture(glm::vec4 _position, glm::vec3 color)
        : position(_position), kColor(color) {}

    virtual ~Fixture() = default;
    virtual inline void Update(float dt) = 0;
    virtual void Translate(const glm::vec3& translation_vector) = 0;

    virtual inline void ComputeVertexPoints() = 0;

    std::vector<glm::vec4> Vertices() { return vertices; }

    glm::vec3 Color() { return kColor; }
    glm::vec4 Position() { return position; }

    auto size() { return vertices.size(); }

  protected:
    const glm::vec3 kColor;

    // Represents the current position of the fixture.
    glm::vec4 position;

    // Represents the vertices of the fixture.
    std::vector<glm::vec4> vertices;
};

#endif /* fixture_hpp */
