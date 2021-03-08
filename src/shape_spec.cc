#include <lattice/colors.h>
#include <lattice/shape_spec.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

ShapeSpec::ShapeSpec() {
    const std::string spec_file = "sim_specs/cube.spec";
    std::vector<std::string> lines;

    std::ifstream input{spec_file};
    if (!input) {
        std::cerr << "No shape spec input specified." << std::endl;
        exit(1);
    }

    std::string current_line;
    while (std::getline(input, current_line)) {
        lines.push_back(current_line);
    }
    this->lines = lines;

    Parse();
}

void ShapeSpec::Parse() {
    MSS sim;

    for (;;) {
        // Ignore whitespace
        if (lines[current_line] == "" || lines[current_line] == " ") {
            ++current_line;
            continue;
        }

        if (Peek(kKeywordName)) {
            if (sim.masses.size() > 0) {
                sim_objects.push_back(sim);
            }

            sim = {};
            ++current_line;
            ParseName(sim);
            ++current_line;
        } else if (Peek(kKeywordVertices)) {
            ++current_line;
            while (!Peek(kKeywordPositions)) {
                ParseVertices(sim);
                ++current_line;
            }

            ++current_line;

            int end_line = current_line + sim.masses.size();
            // Need a position for each mass
            while (current_line < end_line) {
                ParsePositions(sim);
                ++current_line;
            }
        } else { // End of the file
            sim_objects.push_back(sim);
            break;
        }
    }
}

void ShapeSpec::ParseName(MSS& sim) {
    const std::string line = lines[current_line];
    // Line should always just be the name
    sim.name = line;
}

void ShapeSpec::ParsePositions(MSS& sim) {
    const auto line = lines[current_line];
    const auto xyz = Split(line);

    if (xyz.size() < PositionValues::Z) {
        std::cerr << "Parse error at line: " << current_line
                  << ", missing coordinate value" << std::endl;
    }

    const float x = std::stof(xyz[PositionValues::X]);
    const float y = std::stof(xyz[PositionValues::Y]);
    const float z = std::stof(xyz[PositionValues::Z]);

    sim.positions.push_back(Eigen::Vector3f(x, y, z));
}

void ShapeSpec::ParseVertices(MSS& sim) {
    const auto line = lines[current_line];
    const auto params = Split(line);

    if (params.size() < VertexParameters::ADJACENCIES) {
        std::cerr << "Parse error at line: " << current_line
                  << ", missing parameter" << std::endl;
    }

    auto name = params[VertexParameters::NAME];
    auto color = colors::from_string(params[VertexParameters::COLOR]);
    auto fixed = params[VertexParameters::FIXED] == "Fixed";

    std::vector<std::string> adjacencies;

    for (int i = VertexParameters::ADJACENCIES; i < params.size(); ++i) {
        adjacencies.push_back(params[i]);
    }

    sim.masses.push_back(MassNode{
        .name = name,
        .color = color,
        .fixed = fixed,
        .adjacencies = adjacencies,
    });
}

// ===========================================================
bool ShapeSpec::Peek(const std::string& next) {
    return lines[current_line] == next;
}

bool ShapeSpec::StringStartsWith(const std::string& s,
                                 const std::string& start) {
    return s.rfind(start, 0) == 0;
}

std::vector<std::string> ShapeSpec::Split(const std::string& s) {
    std::istringstream iss(s);
    return std::vector<std::string>(std::istream_iterator<std::string>{iss},
                                    std::istream_iterator<std::string>());
}
