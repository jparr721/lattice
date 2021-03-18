#include <lattice/configuration.h>

MSSConfig::MSSConfig(std::string _name, int _width, int _height, int _depth)
    : name(std::move(_name)), depth(_depth), height(_height), width(_width) {
    Initialize();
}

void MSSConfig::Initialize() {
    assert(width > 0);
    assert(depth > 0);
    assert(height > 1);

    const int total_masses = width * height * depth;
    const int z_index_bucket_size = total_masses / width;

    // Make a uniform distribution of x values around the center.
    const auto x_distribution = UniformCoordinateDistribution(width);
    auto z_distribution = UniformCoordinateDistribution(depth);
    std::reverse(z_distribution.begin(), z_distribution.end());

    assert(total_masses % x_distribution.size() == 0);

    std::vector<MassNode> _masses;
    std::vector<Eigen::Vector3f> _positions;

    const int mod = width * height;
    int group = z_index_bucket_size;
    int z_index = 0;
    for (int i = 0; i < total_masses; ++i) {

        if (group == 0) {
            ++z_index;
            group = z_index_bucket_size;
        }

        const int x = x_distribution[(i % mod) % x_distribution.size()];
        const int z = z_distribution[z_index];

        const bool fixed = i % mod > width - 1;
        const auto position = Eigen::Vector3f(x, 0.f, z);

        node_states[i] = PermutationStateMachine{};
        MassNode mn{
            .number = i,
            .fixed = fixed,
            .color = Colors::kRed,
            .adjacencies = std::vector<int>{},
        };

        AssignRightAdjacency(mn);
        AssignBottomAdjacency(mn);
        AssignBehindAdjacency(mn);

        _positions.push_back(position);
        _masses.push_back(mn);
        --group;
    }

    masses = _masses;
    positions = _positions;
    kLastNodeIndex = masses.size();
}

std::vector<int> MSSConfig::UniformCoordinateDistribution(int n) {
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

void MSSConfig::Permute() {
    if (current_state_machine_node == kTerminalMutationState) {
        return;
    }

    if (current_state_machine_node == kLastNodeIndex) {
        current_state_machine_node = kFirstNodeIndex;
    }

    // Shift the state and prepare to spool the compiler
    node_states[current_state_machine_node].Forward();

    CompileShapeMutation();

    ++current_state_machine_node;
}

void MSSConfig::CompileShapeMutation() {
    auto mass = masses[current_state_machine_node];

    mass.adjacencies.clear();
    AssignRightAdjacency(mass);
    AssignBottomAdjacency(mass);

    switch (node_states[mass.number].current_state) {
    case PermutationStateMachine::PermutationState::NORMAL:
        AssignBehindAdjacency(mass);
        break;
    case PermutationStateMachine::PermutationState::FRONT_DIAGONAL:
        if (IsBackLayerNode(mass)) {
            break;
        }
        AssignFrontDiagonalAdjacency(mass);
        break;
    case PermutationStateMachine::PermutationState::ACROSS_DIAGONAL:
        if (IsBackLayerNode(mass)) {
            break;
        }
        AssignAcrossDiagonalAdjacency(mass);
        break;
    case PermutationStateMachine::PermutationState::DISCONNECTED:
        mass.adjacencies.clear();
        break;
    }

    masses[current_state_machine_node] = mass;
}

void MSSConfig::AssignRightAdjacency(MSSConfig::MassNode& node) const {
    // If the current node is not an end node, assign an adjacent connection.
    if (!IsRightCornerNode(node)) {
        // The right-adjacent node is always <node.number> + 1.
        node.adjacencies.push_back(node.number + 1);
    }
}

void MSSConfig::AssignBottomAdjacency(MSSConfig::MassNode& node) const {

    if (IsTopLayerNode(node)) {
        // The bottom-adjacent node is always <node.number> + width.
        node.adjacencies.push_back(node.number + width);
    }
}

void MSSConfig::AssignBehindAdjacency(MSSConfig::MassNode& node) const {
    int mod = width * height;

    if (depth > 1 && node.fixed) {
        node.adjacencies.push_back((node.number % mod) + mod);
    }
}

/**
 * Right now, this _always_ works by taking a top-level node and applying it
 * to the next node behind it at depth current_node_depth + 1. For example:
 * Side View
 *  * (v0) * (v8)  *  * - Top layers
 *    \
 *     __
 *       \
 *  *(v4) *(v12)  *  * - Bottom layers
 *
 *  So v0 -> v12 is the formed connection. So we always just need to increment
 *  the value by width * height + width since we are doing the first value from
 *  layer one, and adding an entire layer plus the width to get the _next_ node
 *  int the sequence. For bottom nodes, we just need to assign it to
 *  the layer in front and above it, which is just a difference of <width>
 *
 *  Excludes calculation for nodes that are higher depth (last layer)
 * @param node
 */
void MSSConfig::AssignFrontDiagonalAdjacency(MSSConfig::MassNode& node) const {
    if (!IsBackLayerNode(node)) {
        if (IsBottomLayerNode(node)) {
            node.adjacencies.push_back(node.number + width);
        } else {
            node.adjacencies.push_back(node.number +
                                       ((width * height) + width));
        }
    }
}

/**
 * Right now, this _always_ works by taking a top-level node and applying it
 * to the next node behind it at the opposite corner. Only corner nodes can
 * use this connection type right now.
 *
 * In the case of a network of nodes with depth = 4 width = 4
 * and height = 2, we can demonstrate this as:
 * v0 -> v15 for upper left corner nodes
 * v3 -> v12 for upper right corner nodes
 * v4 -> v11 for lower left corner nodes
 * v7 -> v8 for lower right corner nodes
 * @param node
 */
void MSSConfig::AssignAcrossDiagonalAdjacency(MSSConfig::MassNode& node) const {
    if (!IsBackLayerNode(node)) {
        // Bottom Corner Nodes
        if (IsBottomLayerNode(node)) {
            if (IsRightCornerNode(node)) {
                node.adjacencies.push_back(node.number + 1);
            } else if (IsLeftCornerNode(node)) {
                // Difference is one layer - 1 since the current node is a part
                // of that layer.
                node.adjacencies.push_back(node.number + (width * height) - 1);
            }
        } else { // Upper Corner Nodes
            if (IsRightCornerNode(node)) {
                // Difference is one layer + 1 since the current node is a part
                // of that layer.
                node.adjacencies.push_back(node.number + (width * height) + 1);
            } else if (IsLeftCornerNode(node)) {
                // Diff accounts for 0-indexing
                int layer_diff = (2 * (width * height)) - 1;
                node.adjacencies.push_back(node.number + layer_diff);
            }
        }
    }
}

bool MSSConfig::IsBackLayerNode(const MSSConfig::MassNode& node) const {
    return node.number >= (width * height * depth) - (width * height);
}

bool MSSConfig::IsTopLayerNode(const MSSConfig::MassNode& node) const {
    return node.number % (width * height) < width;
}

bool MSSConfig::IsBottomLayerNode(const MSSConfig::MassNode& node) const {
    return !IsTopLayerNode(node);
}

bool MSSConfig::IsRightCornerNode(const MSSConfig::MassNode& node) const {
    return ((node.number % (width * height)) + 1) % width == 0;
}

bool MSSConfig::IsLeftCornerNode(const MSSConfig::MassNode& node) const {
    return node.number % width == 0;
}
