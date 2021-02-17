#pragma once

#include "keyboard.h"

#include <QKeyEvent>

class Camera {
  public:
    float x;
    float y;
    float z;

    Camera() : x(0), y(0), z(-3) {}
    Camera(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    ~Camera() = default;

    void OnKeyPress(QKeyEvent* event);

  private:
    Keyboard keyboard;
};