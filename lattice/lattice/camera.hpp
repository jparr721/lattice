//
//  camera.hpp
//  lattice
//
//  Created by Jarred Parr on 2/7/21.
//

#ifndef camera_hpp
#define camera_hpp

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class Camera {
  public:
    Camera(GLuint shader_program_id);

    glm::mat4 MVP() { return mvp; }
    GLuint MatrixId() { return matrix_id; }

  private:
    glm::mat4 mvp;
    GLuint matrix_id;
};

#endif /* camera_hpp */
