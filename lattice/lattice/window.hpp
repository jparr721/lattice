//
//  window.hpp
//  lattice
//
//  Window is a class for building and managing the openGL context
//  as well as the parameters needed.
//  Created by Jarred Parr on 2/1/21.
//

#ifndef window_hpp
#define window_hpp

#include "vertex.hpp"
#include "keyboard.hpp"

#include <memory>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

class Window {
  public:
    Window(std::unique_ptr<Vertex>& _shape) : shape(std::move(_shape)) {}
    ~Window() = default;

    void Initialize();

    int Run();

  private:
    // Window width
    constexpr static int WIDTH = 1200;

    // Window height
    constexpr static int HEIGHT = 1000;

    // Major OpenGL Version
    constexpr static int MAJOR = 3;

    // Minor OpenGL Version
    constexpr static int MINOR = 3;

    // Window Title
    constexpr static const char* TITLE = "Mass Sim";

    // Flag if the class was initialized
    bool is_init = false;

    // Window Reference
    GLFWwindow* window = nullptr;

    // Shapes
    // TODO(@jparr721) - Update this to use a base class (springs, vertices)
    std::unique_ptr<Vertex> shape;
    
    // Keyboard input handler
    std::unique_ptr<Keyboard> keyboard;
};


void OnKeyboardButtonPress(GLFWwindow* window, int key, int scancode,
                           int action, int mods);

#endif /* window_hpp */
