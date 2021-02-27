#pragma once

#include "keyboard.h"

#include <QKeyEvent>
#include <QVector3D>
#include <QApplication>

class Camera {
  public:
    float x;
    float y;
    float z;
    float rot = 0;
    QVector3D direction = QVector3D(0, 0, 0);

    Camera() : x(0), y(0), z(-50) {}
    ~Camera() = default;

    void OnKeyPress(QKeyEvent* event);

  private:
    Keyboard keyboard;

    void OnRotate(int key);
    void OnTranslate(int key);
};
