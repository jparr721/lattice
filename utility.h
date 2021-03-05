#pragma once

#include <Eigen/Dense>

namespace utility {
enum VectorPosition {
    X = 0,
    Y,
    Z,
    W,
};

float EuclideanDistance(const Eigen::Vector3f& left,
                        const Eigen::Vector3f& right);
} // namespace utility
