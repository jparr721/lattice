#include "utility.h"

namespace utility {
float EuclideanDistance(const Eigen::Vector4f& left,
                        const Eigen::Vector4f& right) {
    const auto lx = left(0);
    const auto ly = left(1);
    const auto lz = left(2);

    const auto rx = right(0);
    const auto ry = right(1);
    const auto rz = right(2);

    return std::sqrt((lx - rx) + (ly - ry) + (lz - rz));
}
} // namespace utility
