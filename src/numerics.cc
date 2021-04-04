//
// Created by Jarred Parr on 4/3/21.
//

#include <lattice/toolbox/numerics.h>

namespace toolbox::numerics {
    float LinearInterpolation(float v0, float v1, float t) {
        return v0 + t * (v1 - v0);
    }
}