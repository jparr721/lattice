#include "keyboard.h"

#include <iostream>

bool Keyboard::IsValidKey(int key) {
    if (key_states.find(key) == key_states.end()) {
        std::cout << "Invalid key press: " << key << std::endl;
        return false;
    }

    return true;
}

int Keyboard::OnKeyPressed(int key) {
    if (IsValidKey(key)) {
        key_states[key] = true;
        return key;
    }

    return -1;
}

int Keyboard::OnKeyReleased(int key) {
    if (IsValidKey(key)) {
        key_states[key] = false;
        return key;
    }

    return -1;
}