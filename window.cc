#include "window.h"
#include "mass.h"

#include <QVector3D>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

std::string Window::ReadVertexShader() {
    std::ostringstream sstr;
    auto stream = std::ifstream{"core.vs"};
    sstr << stream.rdbuf();
    return sstr.str();
}

std::string Window::ReadFragmentShader() {
    std::ostringstream sstr;
    auto stream = std::ifstream{"core.frag"};
    sstr << stream.rdbuf();
    return sstr.str();
}

void Window::initialize() {
    const auto vertex_shader = ReadVertexShader();
    const auto fragment_shader = ReadFragmentShader();

    program_id = new QOpenGLShaderProgram(this);
    program_id->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                        vertex_shader.data());
    program_id->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                        fragment_shader.data());
    program_id->link();
    position = program_id->attributeLocation("position_attribute");
    Q_ASSERT(position != -1);
    color = program_id->attributeLocation("color_attribute");
    Q_ASSERT(color != -1);
    matrix_uniform = program_id->uniformLocation("projection_matrix");
    Q_ASSERT(matrix_uniform != -1);
}

void Window::render() {
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    program_id->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);

    program_id->setUniformValue(matrix_uniform, matrix);

    auto mass = std::make_unique<Mass>();
    mass->Translate(QVector3D(1.f, 1.f, 0.f));

    std::vector<QVector3D> vertices{{
        QVector3D(-.2f, -.2f, 0.f),
        QVector3D(.2f, -.2f, 0.f),
        QVector3D(0.f, .2f, 0.f),
        QVector3D(-1.2f, 1.2f, 0.f),
        QVector3D(-1.f, 1.f, 0.f),
        QVector3D(0.f, 1.4f, 0.f),
    }};

    std::vector<QVector3D> colors{{
        QVector3D(1.0f, 0.f, 0.f),
        QVector3D(1.0f, 0.f, 0.f),
        QVector3D(1.0f, 0.f, 0.f),

        QVector3D(0.0f, 1.f, 0.f),
        QVector3D(0.0f, 1.f, 0.f),
        QVector3D(0.0f, 1.f, 0.f),
    }};

    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0,
                          static_cast<void*>(vertices.data()));
    glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 0,
                          static_cast<void*>(colors.data()));

    glEnableVertexAttribArray(position);
    glEnableVertexAttribArray(color);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glDisableVertexAttribArray(color);
    glDisableVertexAttribArray(position);

    program_id->release();

    ++frame;
}