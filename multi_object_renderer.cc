#include "multi_object_renderer.h"

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
        for (auto adjacency : adjacencies) {
            /* auto spring = std::make_shared<Spring>( */
            /*     1.0f, 0.5f, colors::kGreen, */
        }
    }
}
