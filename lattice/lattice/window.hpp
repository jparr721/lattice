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

#include <memory>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

class Window {
public:
    Window() = default;
    ~Window() = default;
    
    void Initialize();
    
    int Run() const;
private:
    // Window width
    constexpr static int WIDTH = 800;
    
    // Window height
    constexpr static int HEIGHT = 600;
    
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
};

#endif /* window_hpp */
