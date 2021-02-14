#include "gl_window.h"

#include <QGuiApplication>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QtMath>

class Window : public GLWindow {
  public:
    void initialize() override;
    void render() override;

  private:
    GLint position = 0;
    GLint color = 0;
    GLint matrix_uniform = 0;

    QOpenGLShaderProgram* program = nullptr;

    int frame = 0;

    std::string ReadVertexShader();
    std::string ReadFragmentShader();
};
