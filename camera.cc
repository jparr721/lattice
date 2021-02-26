#include "camera.h"

#include <iostream>

void Camera::OnKeyPress(QKeyEvent* event) {
    const auto key = event->key();
    if (keyboard.OnKeyPressed(key) > 0) {
        switch (key) {
        case Keyboard::kForward:
            z += 0.1;
            break;
        case Keyboard::kBackward:
            z -= 0.1;
            break;
        case Keyboard::kLeft:
            x += 0.1;
            break;
        case Keyboard::kRight:
            x -= 0.1;
            break;
        case Keyboard::kUp:
            y -= 0.1;
            break;
        case Keyboard::kDown:
            y += 0.1;
            break;
        default:
            break;
        }
    }
    keyboard.OnKeyReleased(key);
}

void Camera::OnMousePress(const Eigen::Vector3f& pos) {
    rot = pos.sum();
    translation = QVector3D(pos.x(), pos.y(), pos.z());
}
