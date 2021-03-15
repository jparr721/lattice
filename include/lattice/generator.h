#pragma once

#include <lattice/colors.h>

#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <Eigen/Dense>

class PermutationStateMachine {
  public:
    enum class PermutationState {
        NORMAL = 0x00,
        FRONT_DIAGONAL,
        ACROSS_DIAGONAL,
        DISCONNECTED,
    };

    void Forward() {
        switch (current_state) {
        case PermutationState::NORMAL:
            current_state = PermutationState::FRONT_DIAGONAL;
        case PermutationState::FRONT_DIAGONAL:
            current_state = PermutationState::ACROSS_DIAGONAL;
        case PermutationState::ACROSS_DIAGONAL:
            current_state = PermutationState::DISCONNECTED;
        case PermutationState::DISCONNECTED:
            break;
        }
    }

    void Backward() {
        switch (current_state) {
        case PermutationState::NORMAL:
            break;
        case PermutationState::FRONT_DIAGONAL:
            current_state = PermutationState::NORMAL;
        case PermutationState::ACROSS_DIAGONAL:
            current_state = PermutationState::FRONT_DIAGONAL;
        case PermutationState::DISCONNECTED:
            current_state = PermutationState::ACROSS_DIAGONAL;
        }
    }

    bool IsTerminalState() {
        return current_state > PermutationState::ACROSS_DIAGONAL;
    }

  private:
    PermutationState current_state = PermutationState::NORMAL;
};

class MSSConfig {
  private:
    struct MassNode {
        int number;
        bool fixed;
        Eigen::Vector3f color;
        std::vector<int> adjacencies;
    };

  public:
    const int width;
    const int height;
    const int depth;

    const Eigen::Vector3f color = Colors::kRed;

    std::string name;
    std::vector<MassNode> masses;
    std::vector<Eigen::Vector3f> positions;

    MSSConfig(std::string _name, int _width, int _height, int _depth);

    void Initialize();

    static std::vector<int> UniformCoordinateDistribution(int n);

    void Permute();

  private:
    constexpr static int kDefaultMidpoint = 0;
    constexpr static int kMinimumSeparationDistance = 4;
    constexpr static int kFirstNodeIndex = 0;
    constexpr static int kTerminalMutationState = -1;

    int kLastNodeIndex;

    int last_mutated_node = kFirstNodeIndex;

    std::unordered_map<int, PermutationStateMachine> node_states;

    void CompileShapeMutation();
};