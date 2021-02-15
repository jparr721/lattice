//
//  window.cpp
//  lattice
//
//  Created by Jarred Parr on 2/1/21.
//

#include <iostream>

#include "keyboard.hpp"
#include "shader.hpp"
#include "window.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

void Window::Initialize() {
    // Set up basic GLFW Parameters
    DeclareGLFWConfigs();

    // Set up our window context variables and screen size
    ConfigureWindowContext();

    // Set up to handle keyboard input
    SetupKeyboardHandler();

    // Load shaders from our GLSL files
    LoadShaders();

    // Create the camera object and position it
    SetupCamera();

    // If we get through, we are all set
    is_init = true;
}

void Window::DeclareGLFWConfigs() {
    // Initialize GLFW
    glfwInit();

    // Set Version for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // MacOS Specific Compatability Setting
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Set window to not be resizable
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "GLFW window create failed :(" << std::endl;
        glfwTerminate();
        return;
    }
}

void Window::ConfigureWindowContext() {
    int screenWidth;
    int screenHeight;

    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    glfwMakeContextCurrent(window);

    // Turn on experimental so GLEW knows to use modern functionality
    glewExperimental = GL_TRUE;

    // Start up OpenGL/Glew
    assert(GLEW_OK == glewInit() && "Failed to initialize GLEW");

    // Set viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);
}

void Window::SetupKeyboardHandler() {
    glfwSetWindowUserPointer(window, this);

    auto keyboard_handler = [](GLFWwindow* w, int k, int s, int a, int m) {
        auto ww = (Window*)glfwGetWindowUserPointer(w);
        TakeAction(ww->masses, k, a);
    };

    // Set up to handle keybindings
    glfwSetKeyCallback(window, keyboard_handler);
}

void Window::LoadShaders() {
    // Load Shaders
    auto shader = std::make_unique<Shader>();
    shader_program_id = shader->Initialize("core.vs", "core.frag");
}

void Window::SetupCamera() {
    camera = std::make_unique<Camera>(shader_program_id);
}

void Window::Display() {
    // Check for events being activated.
    glfwPollEvents();

    // Render setup with shaders
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Configure our shaders
    glUseProgram(shader_program_id);

    // Configure the camera transform into the shader
    glUniformMatrix4fv(camera->MatrixId(), 1, GL_FALSE, &camera->MVP()[0][0]);

    // =========================
    // Mass Object Render Loop
    masses->Render();
    glBindVertexArray(masses->vertex_array_object);

    // Draw value from points 0-3 (x, y z)
    glDrawArrays(masses->RenderMode(), 0, masses->size());
    glBindVertexArray(0);

    // Update only the moving mass with the simulation timestep
    masses->Update(simulation_timestep);

    // Swap the screen buffers
    glfwSwapBuffers(window);
}

int Window::Run() {
    masses->Initialize();

    // Start main window loop and spawn the window
    while (!glfwWindowShouldClose(window)) {
        Display();
    }

    glDeleteVertexArrays(1, &masses->vertex_array_object);
    glDeleteBuffers(1, &masses->vertex_buffer_object);

    glfwTerminate();

    return EXIT_SUCCESS;
}
