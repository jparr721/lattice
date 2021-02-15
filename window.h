#pragma once

#include "gl_window.h"
#include "mass_spring_system.h"

#include <QGuiApplication>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <cmath>
#include <memory>

class Window : public GLWindow {
  public:
    using GLWindow::GLWindow;

    void initialize() override;
    void render() override;

  private:
    GLint position = 0;
    GLint color = 0;
    GLint matrix_uniform = 0;

    QOpenGLShaderProgram* program_id = nullptr;
    int frame = 0;

    std::shared_ptr<MassSpringSystem> mass_spring_system;

    std::string ReadVertexShader();
    std::string ReadFragmentShader();
};