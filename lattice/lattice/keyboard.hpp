//
//  keyboard.hpp
//  lattice
//
//  Created by Jarred Parr on 2/1/21.
//

#ifndef keyboard_hpp
#define keyboard_hpp

#include <memory>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include "vertex.hpp"

class Keyboard {
public:
    Keyboard(GLFWwindow* window, int _key, int _action) : key(_key), action(_action) {}
    
    void TakeAction(Vertex& vertex);
private:
    const int key;
    const int action;
    
    void OnUpKeyPress(Vertex& vertex);
    void OnDownKeyPress();
    void OnLeftKeyPress();
    void OnRightKeyPress();
};

void OnKeyboardButtonPress(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif /* keyboard_hpp */
