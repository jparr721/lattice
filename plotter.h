#pragma once

#include <vector>
#include <string>

/**
 * @brief Plotter is for keeping track of the force, velocity, and position values
 * over time and writing them to a file for plotting in python.
 *
 */
namespace plotter {
class Plotter {
  public:
    struct Position {
        float x;
        float y;

        Position(float _x, float _y) : x(_x), y(_y) {}

        std::string to_string() { return "(" + std::to_string(x) + "," + std::to_string(y) + ")"; }
    };

    Plotter() = default;
    ~Plotter() = default;

    void AddVelocity(float velocity);
    void AddPosition(Position pos);
    void AddForce(float force);
    void DropToFile(std::string file);

  private:
    std::vector<float> forces;
    std::vector<float> velocities;
    std::vector<Position> positions;

    std::string s_forces;
    std::string s_velocities;
    std::string s_positions;

    void Serialize();
};
} // namespace plotter