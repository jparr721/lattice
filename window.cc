#include "window.h"

#include <fstream>
#include <sstream>
#include <vector>

std::string Window::ReadVertexShader() {
    const std::string path = "./shaders/core.vs";
    std::ostringstream sstr;
    auto stream = std::ifstream{path.data()};
    sstr << stream.rdbuf();
    return sstr.str();
}

std::string Window::ReadFragmentShader() {
    const std::string path = "./shaders/core.frag";
    std::ostringstream sstr;
    auto stream = std::ifstream{path.data()};
    sstr << stream.rdbuf();
    return sstr.str();
}

void Window::initialize() {
    const auto vertex_shader_string = ReadVertexShader();
    const auto fragment_shader_string = ReadFragmentShader();

    program = new QOpenGLShaderProgram(this);
    program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                     vertex_shader_string.c_str());
    program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                     fragment_shader_string.c_str());

    program->link();
    position = program->attributeLocation("position_attribute");
    Q_ASSERT(position != -1);

    color = program->attributeLocation("color_attribute");
    Q_ASSERT(color != -1);

    matrix_uniform = program->attributeLocation("projection_matrix");
    Q_ASSERT(matrix_uniform != -1);
}

void Window::render() {
    const qreal retina_scale = devicePixelRatio();
    glViewport(0, 0, width() * retina_scale, height() * retina_scale);

    glClear(GL_COLOR_BUFFER_BIT);

    program->bind();

    QMatrix4x4 matrix;

    // 4:3 aspect ratio, display 0.1 unit to 100 units
    matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    // Move back 5 units so we can have a bigger FOV
    matrix.translate(0, 0, -5);

    program->setUniformValue(matrix_uniform, matrix);

    std::vector<GLfloat> vertices{{
        -.2f, -.2f, 0.f, // Bottom Left
        .2f, -.2f, 0.f,  // Bottom Right
        0.f, .2f, 0.f,   // Top Center

        -1.2f, 1.2f, 0.f, // Bottom Left
        -1.f, 1.f, 0.f,   // Bottom Right
        0.f, 1.4f, 0.f    // Top Center
    }};

    std::vector<GLfloat> colors{{
        1.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,

        1.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
    }};

    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0,
                          static_cast<void*>(vertices.data()));
    glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 0,
                          static_cast<void*>(colors.data()));
}