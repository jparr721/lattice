#pragma once

#include "gl_window.h"
#include "mass.h"

#include <memory>
#include <QGuiApplication>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QtMath>

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

    std::unique_ptr<Mass> mass;

    std::string ReadVertexShader();
    std::string ReadFragmentShader();
};