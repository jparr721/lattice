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

#include "mass_spring_system.hpp"

void TakeAction(const std::shared_ptr<MassSpringSystem>& shape, int key,
                int action);

void OnUpKeyPress(const std::shared_ptr<MassSpringSystem>& shape);
void OnDownKeyPress(const std::shared_ptr<MassSpringSystem>& shape);
void OnLeftKeyPress(const std::shared_ptr<MassSpringSystem>& shape);
void OnRightKeyPress(const std::shared_ptr<MassSpringSystem>& shape);

#endif /* keyboard_hpp */
