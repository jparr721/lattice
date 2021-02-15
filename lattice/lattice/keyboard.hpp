//
//  keyboard.hpp
//  lattice
//
//  A functional API for handling keyboard inputs
//  Created by Jarred Parr on 2/1/21.
//

#ifndef keyboard_hpp
#define keyboard_hpp

#include <memory>

#include "shape.hpp"

void TakeAction(const std::shared_ptr<Shape>& shape, int key, int action);

void OnUpKeyPress(const std::shared_ptr<Shape>& shape);
void OnDownKeyPress(const std::shared_ptr<Shape>& shape);
void OnLeftKeyPress(const std::shared_ptr<Shape>& shape);
void OnRightKeyPress(const std::shared_ptr<Shape>& shape);

#endif /* keyboard_hpp */
