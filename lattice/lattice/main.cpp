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
    const auto window = std::make_unique<Window>();
    window->Initialize();
    window->Run();
}
