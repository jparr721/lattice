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

int Window::Run() {
    int screenWidth;
    int screenHeight;

    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, this);

    auto keyboard_handler = [](GLFWwindow* w, int k, int s, int a, int m) {
        auto ww = (Window*)glfwGetWindowUserPointer(w);
        TakeAction(ww->shape, k, a);
    };

    // Set up to handle keybindings
    glfwSetKeyCallback(window, keyboard_handler);

    // Turn on experimental so GLEW knows to use modern functionality
    glewExperimental = GL_TRUE;

    // Start up OpenGL/Glew
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Set viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);

    // Load Shaders
    auto shader = std::make_unique<Shader>();
    auto program_id = shader->Initialize("core.vs", "core.frag");

    // Get a model view projection matrix handle, named as mvp
    GLuint matrix_id = glGetUniformLocation(program_id, "mvp");

    // Projection matrix: 45 degree FOV, 4:3 aspect, display 0.1unit to 100
    // units
    glm::mat4 projection =
        glm::perspective(glm::radians(100.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    // Set up our camera in the frame
    // TODO(@jparr721) - Constants
    glm::mat4 camera_view =
        glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    // Matrix representing our model
    glm::mat4 model = glm::mat4(1.0f);

    // Model view projection, multiplying our projection, camera view, and the
    // model
    glm::mat4 mvp = projection * camera_view * model;

    shape->Initialize();

    // Start main window loop and spawn the window
    while (!glfwWindowShouldClose(window)) {
        // Check for events being activated.
        glfwPollEvents();

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Configure our shaders
        glUseProgram(program_id);

        // Configure the camera transform into the shader
        glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);

        shape->Render();
        glBindVertexArray(shape->vertex_array_object);

        // Draw value from points 0-3
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &shape->vertex_array_object);
    glDeleteBuffers(1, &shape->vertex_buffer_object);

    glfwTerminate();

    return EXIT_SUCCESS;
}
