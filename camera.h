#pragma once

#include "keyboard.h"

#include <Eigen/Dense>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QVector3D>

class Camera {
  public:
    float x;
    float y;
    float z;
    float rot;
    QVector3D translation;

    Camera() : x(0), y(0), z(-50), rot(0) {}
    ~Camera() = default;

    void OnKeyPress(QKeyEvent* event);
    void OnMousePress(const Eigen::Vector3f& pos);

  private:
    Keyboard keyboard;
};
