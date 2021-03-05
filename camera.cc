#include "camera.h"

#include <iostream>

void Camera::OnKeyPress(QKeyEvent* event) {
    const auto key = event->key();
    const auto shift_modifier_pressed =
        Qt::ShiftModifier == QApplication::keyboardModifiers();

    keyboard.OnKeyPressed(key);

    if (shift_modifier_pressed) {
        OnRotate(key);
    } else {
        OnTranslate(key);
    }

    keyboard.OnKeyReleased(key);
}

void Camera::OnTranslate(int key) {
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

void Camera::OnRotate(int key) {
    switch (key) {
    case Keyboard::kForward:
        direction = QVector3D(0, 0, 1);
        ++rot;
        break;
    case Keyboard::kBackward:
        direction = QVector3D(0, 0, 1);
        --rot;
        break;
    case Keyboard::kLeft:
        direction = QVector3D(1, 0, 0);
        ++rot;
        break;
    case Keyboard::kRight:
        direction = QVector3D(1, 0, 0);
        --rot;
        break;
    case Keyboard::kUp:
        direction = QVector3D(0, 1, 0);
        ++rot;
        break;
    case Keyboard::kDown:
        direction = QVector3D(0, 1, 0);
        --rot;
        break;
    default:
        break;
    }
}
