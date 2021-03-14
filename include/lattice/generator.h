#pragma once

#include <lattice/colors.h>

#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <Eigen/Dense>

namespace generator {

struct MassNode {
    int number;
    bool fixed;
    Eigen::Vector3f color;
    std::vector<int> adjacencies;
};

struct MSSConfig {
    std::string name;
    std::vector<MassNode> masses;
    std::vector<Eigen::Vector3f> positions;

    MSSConfig() = default;
    MSSConfig(std::string  _name, std::vector<MassNode>  _masses,
              std::vector<Eigen::Vector3f>  _positions)
        : name(std::move(_name)), masses(std::move(_masses)), positions(std::move(_positions)) {}
};

MSSConfig Square(std::string name, int width, int height, int depth,
                 const Eigen::Vector3f& color);
constexpr int kDefaultMidpoint = 0;
constexpr int kMinimumSeparationDistance = 4;

std::vector<int> UniformCoordinateDistribution(int n);

} // namespace generator