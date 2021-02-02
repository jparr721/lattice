//
//  point_mass.hpp
//  lattice
//
//  Point Mass sidecars any rigid mass object and calculates
//  all of the forces on its behalf to update.
//  Created by Jarred Parr on 2/1/21.
//

#ifndef point_mass_hpp
#define point_mass_hpp

#include "vec.hpp"

class PointMass {
  public:
    PointMass(int k, int m, vec2 x) : spring_constant(k), mass(m), position(x) {}
    ~PointMass() = default;

    float update(float dt);

    // ================================
    const int spring_constant;
    const int mass;

    // The initial force applied on the object is calculated in the update
    // method.
    int force = 0;

    // The position of the object at the start, respective to time.
    // For now, this is a vec2, we will make this a template class later.
    vec2 position;

    // The velocity the object is moving at, respective to time.
    int velocity;
};

#endif /* point_mass_hpp */
