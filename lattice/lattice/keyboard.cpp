//
//  keyboard.cpp
//  lattice
//
//  Created by Jarred Parr on 2/1/21.
//

#include "keyboard.hpp"

#include <iostream>
#include <glm/glm.hpp>

void Keyboard::TakeAction(const std::unique_ptr<Vertex>& shape, int key, int action) {
    if (action != GLFW_PRESS) {
        return;
    }

    switch (key) {
    case GLFW_KEY_UP:
        OnUpKeyPress(shape);
        return;
    case GLFW_KEY_DOWN:
        OnDownKeyPress(shape);
        return;
    case GLFW_KEY_LEFT:
        OnLeftKeyPress(shape);
        return;
    case GLFW_KEY_RIGHT:
        OnRightKeyPress(shape);
        return;
    default:
        std::cout << "Invalid Key Press: " << key << std::endl;
        return;
    };
}

void Keyboard::OnUpKeyPress(const std::unique_ptr<Vertex>& shape) const {
    auto change = glm::vec3(0.0f, 0.5, 0.0f);
    shape->Transform(change);
}

void Keyboard::OnDownKeyPress(const std::unique_ptr<Vertex>& shape) const {
    auto change = glm::vec3(0.0f, -0.5, 0.0f);
    shape->Transform(change);
}
void Keyboard::OnRightKeyPress(const std::unique_ptr<Vertex>& shape) const {
    auto change = glm::vec3(0.5f, 0.0, 0.0f);
    shape->Transform(change);
}
void Keyboard::OnLeftKeyPress(const std::unique_ptr<Vertex>& shape) const {
    auto change = glm::vec3(-0.5f, 0.0, 0.0f);
    shape->Transform(change);
}
