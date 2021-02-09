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

#include "camera.hpp"
#include "mass.hpp"
#include "shape.hpp"
#include "spring.hpp"

#include <memory>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

class Window {
  public:
    Window(std::shared_ptr<Shape>& _masses, std::shared_ptr<Shape>& _springs)
        : masses(_masses), springs(_springs) {}
    ~Window() = default;

    void Initialize();

    int Run();

  private:
    // Window width
    constexpr static int WIDTH = 1300;

    // Window height
    constexpr static int HEIGHT = 900;

    // Major OpenGL Version
    constexpr static int MAJOR = 3;

    // Minor OpenGL Version
    constexpr static int MINOR = 3;

    // Window Title
    constexpr static const char* TITLE = "Mass Sim";

    GLuint shader_program_id;

    // Flag if the class was initialized
    bool is_init = false;

    // Our movable mass object
    std::shared_ptr<Shape> masses;

    // Our spring object
    std::shared_ptr<Shape> springs;

    // Our camera object
    std::unique_ptr<Camera> camera;

    // Window Reference
    GLFWwindow* window = nullptr;

    // Our update timestep
    static constexpr float simulation_timestep = 0.01;

    void DeclareGLFWConfigs();
    void ConfigureWindowContext();
    void SetupKeyboardHandler();
    void LoadShaders();
    void SetupCamera();
    void Display();
};

#endif /* window_hpp */
