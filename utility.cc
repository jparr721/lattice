#include "utility.h"

#include <cmath>
#include <iostream>

namespace utility {
float EuclideanDistance(const Eigen::Vector3f& left,
                        const Eigen::Vector3f& right) {
    const auto lx = left(0);
    const auto ly = left(1);
    const auto lz = left(2);

    const auto rx = right(0);
    const auto ry = right(1);
    const auto rz = right(2);

    const auto diff =
        std::pow(lx - rx, 2) + std::pow(ly - ry, 2) + std::pow(lz - rz, 2);

    return std::sqrt(diff);
}
} // namespace utility
