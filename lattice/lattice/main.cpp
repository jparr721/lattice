//
//  main.cpp
//  lattice
//
//  Created by Jarred Parr on 2/1/21.
//

#include <iostream>
#include <memory>

#include "window.hpp"

int main() {
    auto mass = std::make_shared<Mass>();

    const auto window = std::make_unique<Window>(mass);
    window->Initialize();
    window->Run();
}
