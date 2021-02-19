#include "shape_spec.h"
#include "colors.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

ShapeSpec::ShapeSpec() {
    const std::string spec_file = "sim_specs/cube.spec";
    std::vector<std::string> lines;

    std::ifstream input{spec_file};
    if (!input) {
        std::cerr << "No input specified." << std::endl;
        exit(1);
    }

    std::string current_line;
    while (std::getline(input, current_line)) {
        lines.push_back(current_line);
    }

    Parse(lines);
}

void ShapeSpec::Parse(const std::vector<std::string>& lines) {
    bool shape_matched = false;

    int line_number = 1;
    for (auto line : lines) {
        if (StringStartsWith(line, kKeywordVertex)) {
            ParseVertex(line.substr(kKeywordVertex.size()), line_number);
        } else {
            std::cerr << "Parse error at line: " << line_number
                      << ". Invalid specifier " << line << std::endl;
        }
        ++line_number;
    }
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

void ShapeSpec::ParseVertex(const std::string& parameters_string,
                            int line_number) {

    auto params = Split(parameters_string);
    if (params.size() < VertexParameters::ADJACENCIES) {
        std::cerr << "Parse error at line: " << line_number
                  << ", missing parameter" << std::endl;
    }

    auto name = params[VertexParameters::NAME];
    auto color = colors::from_string(params[VertexParameters::COLOR]);
    auto size = std::stof(params[VertexParameters::SIZE]);
    auto fixed = params[VertexParameters::FIXED] == "Fixed";

    std::vector<std::string> adjacencies;

    for (int i = VertexParameters::ADJACENCIES; i < params.size(); ++i) {
        adjacencies.push_back(params[i]);
    }

    masses.push_back(MassNode{
        .name = name,
        .color = color,
        .size = size,
        .fixed = fixed,
        .adjacencies = adjacencies,
    });
}

void ShapeSpec::ComputeDiGraph() {
    for (auto mass : masses) {
        std::vector<MassNode> adjacent_mass_nodes;

        for (auto adjacency : mass.adjacencies) {
            const auto node = FindByName(adjacency);

            // Die immediately if the mass node not found
            assert(node != std::nullopt && "Invalid Adjacency Node Name");

            adjacent_mass_nodes.push_back(node.value());
        }

        graph[mass] = adjacent_mass_nodes;
    }
}

std::optional<MassNode> ShapeSpec::FindByName(const std::string& name) {
    for (auto mass : masses) {
        if (mass.name == name) {
            return mass;
        }
    }
    return std::nullopt;
}
