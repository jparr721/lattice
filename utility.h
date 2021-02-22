#pragma once

#include <Eigen/Dense>

namespace utility {
enum VectorPosition {
    X = 0,
    Y,
    Z,
    W,
};

float EuclideanDistance(const Eigen::Vector4f& left,
                        const Eigen::Vector4f& right);
} // namespace utility
