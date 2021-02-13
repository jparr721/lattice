//
//  keyboard.cpp
//  lattice
//
//  Created by Jarred Parr on 2/1/21.
//

#include "keyboard.hpp"

#include <glm/glm.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

void TakeAction(const std::shared_ptr<MassSpringSystem>& shape, int key,
                int action) {
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

void OnUpKeyPress(const std::shared_ptr<MassSpringSystem>& shape) {
    auto change = glm::vec3(0.0f, 0.5, 0.0f);
    shape->MoveFixtureAtIndex(0, change);
}

void OnDownKeyPress(const std::shared_ptr<MassSpringSystem>& shape) {
    auto change = glm::vec3(0.0f, -0.5, 0.0f);
    shape->MoveFixtureAtIndex(0, change);
}
void OnRightKeyPress(const std::shared_ptr<MassSpringSystem>& shape) {
    auto change = glm::vec3(0.5f, 0.0, 0.0f);
    shape->MoveFixtureAtIndex(0, change);
}
void OnLeftKeyPress(const std::shared_ptr<MassSpringSystem>& shape) {
    auto change = glm::vec3(-0.5f, 0.0, 0.0f);
    shape->MoveFixtureAtIndex(0, change);
}
