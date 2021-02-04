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
    auto shape = Vertex();
    std::vector<Vertex> shapes{shape};

    const auto window = std::make_unique<Window>(shapes);
    window->Initialize();
    window->Run();
}
