#include "multi_object_renderer.h"

#include <optional>

#include <QVector4D>

void MultiObjectRenderer::Initialize() {
    auto shape_spec = std::make_unique<ShapeSpec>();

    // For now, require the graph to be even length so we can partition
    // to make a nice square object, we'll change this later...
    assert(shape_spec->graph.size() % 2 == 0);

    int i = 0;

    const auto top = QVector4D(-0.5f, 1.f, 0.f, 1.f);
    const auto bottom = QVector4D(-0.5f, -1.f, 0.f, 1.f);

    for (const auto& [node, adjacencies] : shape_spec->graph) {
        // Render top 4 nodes and springs
        if (i < 4) {
            auto mass = std::make_shared<Mass>(
                node.size, node.name, node.fixed, node.color,
                // TODO(@jparr721) - This is trash, refactor.
                QVector4D(top.x() + (i * 0.2), top.y(), top.z(), top.w()));
            mass_spring_system->AddMass(std::move(mass));
        } else {
            auto mass = std::make_shared<Mass>(
                node.size, node.name, node.fixed, node.color,
                // TODO(@jparr721) - This is trash, refactor.
                QVector4D(bottom.x() + (i * 0.2), bottom.y(), bottom.z(),
                          bottom.w()));
            mass_spring_system->AddMass(std::move(mass));
        }
        ++i;
    }

    for (const auto& [node, adjacencies] : shape_spec->graph) {
        if (adjacencies.size() > 0) {
            // This'll break when we add more complex shapes!
            const MassNode left_mass_node = adjacencies[0];
            const MassNode right_mass_node = adjacencies[1];

            auto left_adjacent_node =
                mass_spring_system->GetMassByName(left_mass_node.name);
            auto right_adjacent_node =
                mass_spring_system->GetMassByName(right_mass_node.name);

            assert(left_adjacent_node != std::nullopt);
            assert(right_adjacent_node != std::nullopt);

            auto spring = std::make_shared<Spring>(1.0f, 0.5f, colors::kGreen,
                                                   left_adjacent_node.value(),
                                                   right_adjacent_node.value());
            mass_spring_system->AddSpring(std::move(spring));
        }
    }

    is_init = true;
}
