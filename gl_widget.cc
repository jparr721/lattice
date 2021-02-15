#include "colors.h"
#include "gl_widget.h"
#include "mass.h"
#include "spring.h"
#include "window.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QOpenGLShaderProgram>
#include <QPainter>
#include <QVector3D>

GLWidget::GLWidget(QWidget* parent) : QOpenGLWidget(parent) {}
GLWidget::~GLWidget() { delete program_id; }

QSize GLWidget::minimumSizeHint() const { return QSize(kWidth, kHeight); }

QSize GLWidget::sizeHint() const { return QSize(kWidth, kHeight); }

void GLWidget::Update() { return; }

void GLWidget::SetMass(float value) {
    assert(value > kMinimumMassSliderValue && "Invalid Mass Value");
    slider_mass_value = value;
    emit OnMassChange(value);
    Update();
}

void GLWidget::SetSpringConstant(float value) {
    assert(value > kMinimumSpringConstantSliderValue && "Invalid K Value");
    assert(value < kMaximumSpringConstantSliderValue && "Invalid K Value");

    slider_spring_constant_value = value;
    emit OnSpringConstantChange(value);
    Update();
}

void GLWidget::SetSpringDampingConstant(float value) {
    assert(value > kMinimumDampingSliderValue && "Invalid Damping Value");
    assert(value < kMaximumDampingSliderValue && "Invalid Damping Value");

    slider_damping_constant_value = value;
    emit OnSpringDampingChange(value);
    Update();
}

void GLWidget::SetSpringRestLength(float value) {
    assert(value > kMinimumSpringRestLengthSliderValue &&
           "Invalid Spring Rest Length Value");
    assert(value < kMaximumSpringRestLengthSliderValue &&
           "Invalid Spring Rest Length Value");

    slider_rest_length_value = value;
    emit OnSpringRestLengthChange(value);
    Update();
}

void GLWidget::SetTimeStep(float value) {
    assert(value > kMinimumTimeStepChangeSliderValue &&
           "Invalid Time Step Value");
    assert(value < kMaximumTimeStepChangeSliderValue &&
           "Invalid Time Step Value");

    slider_time_step_value = value;
    emit OnTimeStepChange(value);
    Update();
}

void GLWidget::initializeGL() {
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

void GLWidget::paintGL() {
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

// Don't allow resizing for now.
void GLWidget::resizeGL(int width, int height) { return; }

void GLWidget::keyPressEvent(QKeyEvent* event) { return; }
void GLWidget::keyReleaseEvent(QKeyEvent* event) { return; }

std::string GLWidget::ReadVertexShader() {
    std::ostringstream sstr;
    auto stream = std::ifstream{"core.vs"};
    sstr << stream.rdbuf();
    return sstr.str();
}

std::string GLWidget::ReadFragmentShader() {
    std::ostringstream sstr;
    auto stream = std::ifstream{"core.frag"};
    sstr << stream.rdbuf();
    return sstr.str();
}