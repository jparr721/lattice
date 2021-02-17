#include "plotter.h"

#include <fstream>

namespace plotter {
void Plotter::AddVelocity(float velocity) { velocities.push_back(velocity);  }
void Plotter::AddPosition(Position pos) { positions.push_back(pos); }
void Plotter::AddForce(float force) { forces.push_back(force);  }
void Plotter::Serialize() {
    s_forces.clear();
    s_velocities.clear();
    s_positions.clear();

    for (auto f : forces) {
        s_forces += std::to_string(f) + ",";
    }

    for (auto v : velocities) {
        s_velocities += std::to_string(v) + ",";
    }

    for (auto p : positions) {
        s_velocities += p.to_string() + ",";
    }
}
void Plotter::DropToFile(std::string file) {
    Serialize();

    std::ofstream output(file);
    output << "Forces: \n";
    output << s_forces;
    output << "\n";

    output << "Velocities: \n";
    output << s_velocities;
    output << "\n";

    output << "Positions: \n";
    output << s_positions;
    output << "\n";
}
} // namespace plotter