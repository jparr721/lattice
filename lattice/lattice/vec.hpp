//
//  vec.hpp
//  lattice
//
//  Created by Jarred Parr on 2/1/21.
//

#ifndef vec_hpp
#define vec_hpp

#include <array>

enum class VecPosition { X = 0, Y, Z };

template <unsigned dim> struct vec {
    std::array<float, dim> entries;
    
    vec() = default;
    
    float operator[](VecPosition position) const {
        assert(position <= VecPosition::Z && position > VecPosition::X);
        return entries[position];
    }

    // Operator overload for vector-vector
    auto operator*(const vec& rhs) const {
        if constexpr (dim == 2) {
            return entries[VecPosition::X] * rhs[VecPosition::X] +
                   entries[VecPosition::Y] * rhs[VecPosition::Y];
        } else {
            return entries[VecPosition::X] * rhs[VecPosition::X] +
                   entries[VecPosition::Y] * rhs[VecPosition::Y] +
                   entries[VecPosition::Z] * rhs[VecPosition::Z];
        }
    }
    // Operator overload for vector-scalar
    auto operator*(float f) const {
        if constexpr (dim == 2) {
            return entries[VecPosition::X] * f + entries[VecPosition::Y] * f;
        } else {
            return entries[VecPosition::X] * f + entries[VecPosition::Y] * f +
                   entries[VecPosition::Z] * f;
        }
    }
};

typedef vec<2> vec2;
typedef vec<3> vec3;

#endif /* vec_hpp */
