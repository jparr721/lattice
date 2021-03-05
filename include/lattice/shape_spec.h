#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <Eigen/Dense>

enum VertexParameters {
    NAME = 0x00,
    COLOR,
    FIXED,
    ADJACENCIES,
};

enum PositionValues { X = 0x00, Y, Z };

struct MassNode {
    std::string name;
    Eigen::Vector3f color;
    bool fixed;
    std::vector<std::string> adjacencies;

    bool operator==(const MassNode& mn) const {
        return name == mn.name && color == mn.color && fixed == mn.fixed;
    }
};

struct MassNodeHash {
    std::size_t operator()(const MassNode& mn) const {
        return std::hash<std::string>()(mn.name);
    }
};

using spec_graph =
    std::unordered_map<MassNode, std::vector<MassNode>, MassNodeHash>;

struct MSS {
    std::string name;
    std::vector<MassNode> masses;
    std::vector<Eigen::Vector3f> positions;
    spec_graph topology;
};

class ShapeSpec {
  public:
    int current_line = 0;
    std::vector<std::string> lines;

    std::vector<MSS> sim_objects;

    ShapeSpec();
    ~ShapeSpec() = default;

  private:
    inline static const std::string kKeywordComment = "/*";
    inline static const std::string kKeywordName = "name";
    inline static const std::string kKeywordVertices = "vertices";
    inline static const std::string kKeywordPositions = "positions";

    void Parse();
    void ParseVertices(MSS& sim);
    void ParsePositions(MSS& sim);
    void ParseName(MSS& sim);

    bool StringStartsWith(const std::string& s, const std::string& start);
    bool Peek(const std::string& next);

    std::vector<std::string> Split(const std::string& s);

    std::optional<MassNode> FindByName(const std::string& name);
};
