#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <Eigen/Dense>

enum VertexParameters {
    NAME = 0x00,
    SIZE,
    COLOR,
    FIXED,
    ADJACENCIES,
};

struct MassNode {
    std::string name;
    Eigen::Vector3f color;
    float size;
    bool fixed;
    std::vector<std::string> adjacencies;

    bool operator==(const MassNode& mn) const {
        return name == mn.name && color == mn.color && size == mn.size &&
               fixed == mn.fixed;
    }
};

struct MassNodeHash {
    std::size_t operator()(const MassNode& mn) const {
        return std::hash<std::string>()(mn.name);
    }
};

class ShapeSpec {
  public:
    std::unordered_map<MassNode, std::vector<MassNode>, MassNodeHash> graph;

    std::vector<MassNode> masses;

    ShapeSpec();
    ~ShapeSpec() = default;

  private:
    inline static const std::string kKeywordName = "name";
    inline static const std::string kKeywordVertex = "vertex";

    void Parse(const std::vector<std::string>& lines);
    void ParseVertex(const std::string& parameters_string, int line_number);
    void ParseName(const std::string& parameters_string, int line_number);

    void ComputeDiGraph();

    bool StringStartsWith(const std::string& s, const std::string& start);

    std::vector<std::string> Split(const std::string& s);

    std::optional<MassNode> FindByName(const std::string& name);
};
