#pragma once

#include <lattice/colors.h>

#include <optional>
#include <string>
#include <unordered_map>
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
    MSSConfig(const std::string& _name, const std::vector<MassNode>& _masses,
              const std::vector<Eigen::Vector3f> _positions)
        : name(_name), masses(_masses), positions(_positions) {}
};

MSSConfig Square(std::string name, int width, int height, int depth,
                 Eigen::Vector3f color);
constexpr int kDefaultMidpoint = 0;
constexpr int kMinimumSeparationDistance = 4;

std::vector<int> UniformCoordinateDistribution(int n);

} // namespace generator