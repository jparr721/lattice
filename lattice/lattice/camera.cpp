//
//  camera.cpp
//  lattice
//
//  Created by Jarred Parr on 2/7/21.
//

#include "camera.hpp"

#include <glm/gtx/transform.hpp>

Camera::Camera(GLuint shader_program_id) {
    // Get a model view projection matrix handle, named as mvp
    matrix_id = glGetUniformLocation(shader_program_id, "mvp");

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
    mvp = projection * camera_view * model;
}
