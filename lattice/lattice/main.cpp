//
//  main.cpp
//  lattice
//
//  Created by Jarred Parr on 2/1/21.
//

#include <iostream>
#include <memory>

#include "window.hpp"

#include "vertex.hpp"

int main() {
    auto shape = std::make_unique<Vertex>();

    const auto window = std::make_unique<Window>(shape);
    window->Initialize();
    window->Run();
}
