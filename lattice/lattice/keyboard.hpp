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
#include "vertex.hpp"
#include <GLFW/glfw3.h>

class Keyboard {
  public:
    Keyboard() = default;
    ~Keyboard() = default;

    void TakeAction(const std::unique_ptr<Vertex>& shape, int key, int action);

  private:
    void OnUpKeyPress(const std::unique_ptr<Vertex>& shape) const;
    void OnDownKeyPress(const std::unique_ptr<Vertex>& shape) const;
    void OnLeftKeyPress(const std::unique_ptr<Vertex>& shape) const;
    void OnRightKeyPress(const std::unique_ptr<Vertex>& shape) const;
};

#endif /* keyboard_hpp */
