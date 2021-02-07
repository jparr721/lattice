//
//  spring.hpp
//  lattice
//
//  Created by Jarred Parr on 2/6/21.
//

#ifndef spring_hpp
#define spring_hpp

#include "renderable.hpp"
#include "mass.hpp"
#include <memory>

class Spring : public Renderable {
public:
    Spring() : Renderable(1, kRenderableColorBlue), kStiffness(1.f), kLength(5.f), position(glm::vec4(1.f, 1.f, 0.f, 1.f)) {}
    Spring(float stiffness, float resting_length) : Renderable(1, kRenderableColorBlue), kStiffness(stiffness), kLength(resting_length) {}
    
    inline void Render() {
        assert(is_init);
        
        glBufferData(GL_ARRAY_BUFFER, shape.size() * sizeof(float),
                     static_cast<void*>(shape.data()), GL_STATIC_DRAW);
    }
    
    void Translate(const glm::vec3& translation_vector) {
        auto transformation_matrix =
            glm::translate(glm::mat4(1.0f), translation_vector);
        
        // Apply a stretch operation to the vertices
        // TODO
        
        // Recompute the vertices
        ComputeVertexPoints();
        
        // Recompute the shape
        ComputeShapeWithColor();
    }
    
private:
    // The spring stiffness.
    const float kStiffness;
    
    // The resting length of the spring in the Y direction.
    const float kLength;
    
    const std::unique_ptr<Mass> left_mass;
    const std::unique_ptr<Mass> right_mass;
    
    // The position of our rendered spring.
    glm::vec4 position;
    
    void ApplySpringForces();
    // TODO(@jparr721) - Make this a helper.
    float ComputeVectorDistance();
    void ComputeShapeWithColor();
    void ComputeVertexPoints();
};

#endif /* spring_hpp */
