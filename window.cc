#include "window.h"
#include "colors.h"
#include "mass.h"
#include "spring.h"

#include <QVector3D>
#include <fstream>
#include <iostream>
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

    auto fixed_mass =
        std::make_shared<Mass>(0.1, 0.2f, kFixedPosition, colors::kBlue,
                               QVector4D(0.f, 1.f, 0.f, 1.f));
    auto movable_mass =
        std::make_shared<Mass>(0.1, 0.2f, kUnfixedPosition, colors::kRed,
                               QVector4D(0.f, 0.f, 0.f, 1.f));

    auto spring = std::make_shared<Spring>(0.5f, 1.f, colors::kGreen,
                                           fixed_mass, movable_mass);

    mass_spring_system = std::make_unique<MassSpringSystem>();
    mass_spring_system->AddFixture(fixed_mass);
    mass_spring_system->AddFixture(movable_mass);
    mass_spring_system->AddFixture(spring);
    mass_spring_system->Initialize();
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

    auto shapes = mass_spring_system->Shapes();
    auto colors = mass_spring_system->Colors();

    mass_spring_system->Update(0.1f);

    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0,
                          static_cast<void*>(shapes.data()));
    glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 0,
                          static_cast<void*>(colors.data()));

    glEnableVertexAttribArray(position);
    glEnableVertexAttribArray(color);

    glDrawArrays(GL_TRIANGLES, 0, shapes.size());

    glDisableVertexAttribArray(color);
    glDisableVertexAttribArray(position);

    program_id->release();

    ++frame;
}