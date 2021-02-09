//
//  main.cpp
//  lattice
//
//  Created by Jarred Parr on 2/1/21.
//

#include <iostream>
#include <memory>

#include "fixture.hpp"
#include "window.hpp"

int main() {
    auto movable_mass =
        std::make_shared<Mass>(0.2f, 5.f, kUnfixedPosition, colors::kBlue,
                               glm::vec4(0.f, 0.f, 0.f, 0.f));

    auto fixed_mass = std::make_shared<Mass>(
        0.2f, 5.f, kUnfixedPosition, colors::kRed,
        glm::vec4(movable_mass->Position().x + 1.f,
                  movable_mass->Position().y + 1.f, 0.f, 0.f));

    auto mass_fixtures =
        std::vector<std::shared_ptr<Fixture>>{{movable_mass, fixed_mass}};

    auto mass_shapes = std::make_shared<Shape>(mass_fixtures, GL_TRIANGLES);

    auto spring = std::make_shared<Spring>(1.f, 5.f, fixed_mass, movable_mass);

    auto spring_fixtures = std::vector<std::shared_ptr<Fixture>>{{spring}};

    auto spring_shapes = std::make_shared<Shape>(spring_fixtures, GL_LINES);

    const auto window = std::make_unique<Window>(mass_shapes, spring_shapes);
    window->Initialize();
    window->Run();
}
