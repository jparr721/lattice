//
//  point_mass.hpp
//  lattice
//
//  Created by Jarred Parr on 2/1/21.
//

#ifndef point_mass_hpp
#define point_mass_hpp

template<typename T>
class vec3 {
    union {
        struct {
            T x;
            T y;
            T z;
        };
    };
    
    vec3() : x(0), y(0), z(0) {}
    vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    
    inline vec3<T> operator +(const vec3<T>& rhs) const {
        return vec3<T>{x + rhs.x, y + rhs.y, z + rhs.z};
    }
    
    inline vec3<T> operator -(const vec3<T>& rhs) const {
        return vec3<T>{x - rhs.x, y - rhs.y, z - rhs.z};
    }
    
    // Vec -> Scalar
    inline vec3<T> operator*(float f) const {
        return vec3<T>{x * f, y * f, z * f};
    }
    
    // Vec -> Vec
    inline vec3<T> operator *(const vec3<T>& rhs) const {
        return vec3<T>{x * rhs.x, y * rhs.y, z * rhs.z};
    }
};

#endif /* point_mass_hpp */
