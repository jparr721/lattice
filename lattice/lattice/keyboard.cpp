//
//  keyboard.cpp
//  lattice
//
//  Created by Jarred Parr on 2/1/21.
//

#include "keyboard.hpp"

#include <iostream>

void Keyboard::TakeAction(Vertex& vertex) {
    if (action != GLFW_PRESS) {
        return;
    }
    
    switch(key) {
        case GLFW_KEY_UP:
            OnUpKeyPress(vertex);
            return;
        case GLFW_KEY_DOWN:
            OnDownKeyPress();
            return;
        case GLFW_KEY_LEFT:
            OnLeftKeyPress();
            return;
        case GLFW_KEY_RIGHT:
            OnRightKeyPress();
            return;
        default:
            std::cout << "Invalid Key Press: " << key << std::endl;
            return;
    };
}

void Keyboard::OnUpKeyPress(Vertex& vertex) {

}

void Keyboard::OnDownKeyPress() {}
void Keyboard::OnRightKeyPress() {}
void Keyboard::OnLeftKeyPress() {}
