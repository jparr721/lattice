//
//  window.cpp
//  lattice
//
//  Created by Jarred Parr on 2/1/21.
//

#include <iostream>

#include "window.hpp"

// GLEW
//#define GLEW_STATIC
//#include <GL/glew.h>

// GLFW
//#include <GLFW/glfw3.h>

void Window::Initialize() {
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

    // If we get through, we are all set
    is_init = true;
}

int Window::Run() const {
    int screenWidth;
    int screenHeight;

    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    glfwMakeContextCurrent(window);

    // Turn on experimental so GLEW knows to use modern functionality
    glewExperimental = GL_TRUE;

    // Start up OpenGL
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Set viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);

    // Start main window loop and spawn the window
    while (!glfwWindowShouldClose(window)) {
        // Check for events being activated.
        glfwPollEvents();

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}
