#include <lattice/generator.h>

#include <algorithm>
#include <iostream>
#include <utility>

namespace generator {
MSSConfig Square(std::string name, int width, int height, int depth,
                 const Eigen::Vector3f& color) {
    assert(width > 0);
    assert(depth > 0);
    assert(height > 1);

    int total_masses = width * height * depth;

    // Make a uniform distribution of x values around the center.
    const auto x_distribution = UniformCoordinateDistribution(width);
    auto z_distribution = UniformCoordinateDistribution(depth);
    std::reverse(z_distribution.begin(), z_distribution.end());

    assert(total_masses % x_distribution.size() == 0);

    std::vector<MassNode> masses;
    std::vector<Eigen::Vector3f> positions;

    // Render in layers front to back
    int number = 0;
    for (int i = 0; i < depth; ++i) {
        const auto z = z_distribution[i];
        for (int j = 0; j < width * height; ++j) {
            const bool fixed = j > width - 1;

            const auto x = x_distribution[j % x_distribution.size()];

            const auto position = Eigen::Vector3f(x, 0.f, z);

            // Include just the basic "shell" lattice as a start point.
            // This densely connects all masses with no internal structure.
            std::vector<int> adjacencies;

            // "Right"
            // If the node is not a right-edge node.
            if ((j + 1) % width != 0) {
                // The one next to the current node is always one greater
                adjacencies.push_back(number + 1);
            }

            // "Down"
            if (j < width) {
                // The one below the current node is always <width> greater
                adjacencies.push_back(number + width);
            }

            // "Behind"
            if (depth > 1) {
                // The one behind the current node is always <height> * <width>
                // greater
                // adjacencies.push_back(j + (width * height));
            }

            positions.push_back(position);
            masses.push_back({
                .number = number,
                .fixed = fixed,
                .color = color,
                .adjacencies = adjacencies,
            });
            ++number;
        }
    }

    return MSSConfig(std::move(name), masses, positions);
}

std::vector<int> UniformCoordinateDistribution(int n) {
    if (n == 1) {
        return std::vector<int>{kDefaultMidpoint};
    } else if (n == 2) {
        return std::vector<int>{-kMinimumSeparationDistance, kDefaultMidpoint};
    }

    std::vector<int> output;

    int midpoint = n / 2;

    // Adjust left the minimum points per value before center
    int bottom = -kMinimumSeparationDistance * midpoint;
    output.reserve(n);
    for (int i = 0; i < n; ++i) {
        output.push_back(bottom + (4 * i));
    }

    return output;
}
} // namespace generator