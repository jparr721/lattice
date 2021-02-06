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

#include "mass.hpp"

void TakeAction(const std::shared_ptr<Mass>& shape, int key, int action);

void OnUpKeyPress(const std::shared_ptr<Mass>& shape);
void OnDownKeyPress(const std::shared_ptr<Mass>& shape);
void OnLeftKeyPress(const std::shared_ptr<Mass>& shape);
void OnRightKeyPress(const std::shared_ptr<Mass>& shape);

#endif /* keyboard_hpp */
