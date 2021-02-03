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

class PointMass {
  public:
    PointMass()
        : spring_constant(0), mass(1), initial_position(0),
          current_position(0) {}
    PointMass(int k, int m, int x)
        : spring_constant(k), mass(m), initial_position(x),
          current_position(x) {}
    ~PointMass() = default;

    float update(float dt);

    // ================================
    // The k value in hooke's law
    const int spring_constant;

    // The mass of this vertex
    const int mass;

    // The initial force applied on the object is calculated in the update
    // method.
    int force = 0;

    // The position of the object at the start.
    const int initial_position;

    // The position of the object currently with respect to time.
    int current_position;

    // The velocity the object is moving at, respective to time.
    int velocity;
};

#endif /* point_mass_hpp */
