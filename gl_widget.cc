#include "gl_widget.h"
#include "colors.h"
#include "gl_window.h"
#include "mass.h"
#include "spring.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QOpenGLShaderProgram>
#include <QPainter>

GLWidget::GLWidget(QWidget* parent) : QOpenGLWidget(parent) {
    setFocusPolicy(Qt::ClickFocus);

    mass_spring_system = std::make_unique<MassSpringSystem>();

    float updates_per_second = 120;
    float draws_per_second = 30;

    draw_timer = new QTimer(this);
    connect(draw_timer, &QTimer::timeout, this,
            QOverload<>::of(&GLWidget::update));
    draw_timer->start(1000.0 / draws_per_second);

    update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, this, &GLWidget::Update);
    update_timer->start(1000.0 / updates_per_second);
}

void GLWidget::Cleanup() { delete program_id; }

QSize GLWidget::minimumSizeHint() const { return QSize(kWidth, kHeight); }

QSize GLWidget::sizeHint() const { return QSize(kWidth, kHeight); }

void GLWidget::Update() { mass_spring_system->Update(); }

void GLWidget::SetMass(float value) {
    slider_mass_value =
        Interpolate(Mass::kMinimumMassValue, Mass::kMaximumMassValue,
                    (float)value / 100.0f);

    mass_spring_system->SetMassWeight(slider_mass_value);

    emit OnMassChange(value);
    RestartSimulation();
}

void GLWidget::SetSpringConstant(float value) {
    slider_spring_constant_value =
        Interpolate(Spring::kMinimumSpringConstantValue,
                    Spring::kMaximumSpringConstantValue, (float)value / 100.f);
    mass_spring_system->SetSpringStiffness(slider_spring_constant_value);
    emit OnSpringConstantChange(value);
    RestartSimulation();
}

void GLWidget::SetSpringDampingConstant(float value) {
    slider_damping_constant_value =
        Interpolate(Spring::kMinimumDampingValue, Spring::kMaximumDampingValue,
                    (float)value / 100.f);
    mass_spring_system->SetSpringDampingConstant(slider_damping_constant_value);
    emit OnSpringDampingChange(value);
    RestartSimulation();
}

void GLWidget::SetSpringRestLength(float value) {
    slider_rest_length_value = Interpolate(
        Spring::kMinimumSpringRestLengthValue,
        Spring::kMaximumSpringRestLengthValue, (float)value / 100.f);
    mass_spring_system->SetSpringRestLength(value);
    emit OnSpringRestLengthChange(value);
    RestartSimulation();
}

void GLWidget::SetTimeStep(float value) {
    slider_time_step_value = Interpolate(
        MassSpringSystem::kMinimumTimeStepChangeValue,
        MassSpringSystem::kMaximumTimeStepChangeValue, (float)value / 100.f);
    mass_spring_system->SetTimeStep(slider_time_step_value);
    emit OnTimeStepChange(value);
    RestartSimulation();
}

void GLWidget::initializeGL() {
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this,
            &GLWidget::Cleanup);
    initializeOpenGLFunctions();

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

    is_init = true;
}

void GLWidget::paintGL() {
    assert(is_init && "Not Initialized");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program_id->bind();
    QMatrix4x4 matrix;
    matrix.perspective(45.0f, 4.0f / 3.0f, 0.1f, 200.0f);
    matrix.translate(camera.x, camera.y, camera.z);

    // Kinda sucks.. I'll make this translate the shape later instead.
    matrix.rotate(camera.rot, camera.direction);

    program_id->setUniformValue(matrix_uniform, matrix);
    auto shapes = mass_spring_system->Shapes();
    auto colors = mass_spring_system->Colors();

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

void GLWidget::keyPressEvent(QKeyEvent* event) {
    camera.OnKeyPress(event);
    const int key = keyboard.OnKeyPressed(event->key());
    if (key > 0) {
        if (key == Keyboard::kPrint) {
            PrintParameters();
        } else if (key == Keyboard::kRestart) {
            RestartSimulation();
        }
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent* event) {
    keyboard.OnKeyReleased(event->key());
}

std::string GLWidget::ReadVertexShader() {
    std::ostringstream sstr;
    auto stream = std::ifstream{"./shaders/core.vs"};
    sstr << stream.rdbuf();
    return sstr.str();
}

std::string GLWidget::ReadFragmentShader() {
    std::ostringstream sstr;
    auto stream = std::ifstream{"./shaders/core.frag"};
    sstr << stream.rdbuf();
    return sstr.str();
}

float GLWidget::Interpolate(float v0, float v1, float t) {
    return v0 + t * (v1 - v0);
}

void GLWidget::RestartSimulation() {
    mass_spring_system->Reset();

    is_restarted = frame == 0 ? false : true;
}

void GLWidget::PrintParameters() {
    std::cout << "        Parameters:           " << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << "Mass: " << slider_mass_value << std::endl;
    std::cout << "K: " << slider_spring_constant_value << std::endl;
    std::cout << "Damping: " << slider_damping_constant_value << std::endl;
    std::cout << "Rest Length: " << slider_rest_length_value << std::endl;
    std::cout << "==============================" << std::endl;
}

Eigen::Vector4f GLWidget::CurrentSimObjectVelocity() {
    return mass_spring_system->GetFirstMovingMassVelocity();
}

Eigen::Vector4f GLWidget::CurrentSimObjectAcceleration() {
    return mass_spring_system->GetFirstMovingMassAcceleration();
}

Eigen::Vector4f GLWidget::CurrentSimSpringForce() {
    return mass_spring_system->GetFirstSpringForce();
}

bool GLWidget::IsRestarted() {
    if (is_restarted) {
        is_restarted = false;
        return true;
    }

    return false;
}

/**
 * This is here because we are not translating vectors via absolute
 * positions, so when we initially click, we need to set the "last"
 * position vector so we know how much, and in what direction, we've
 * translated"
 */
void GLWidget::mousePressEvent(QMouseEvent* event) {
    last_position = Eigen::Vector3f(event->x(), event->y(), 0.f);
}

void GLWidget::mouseMoveEvent(QMouseEvent* event) {
    int x = event->x();
    int y = event->y();
    Eigen::Vector3f dposition = Eigen::Vector3f(x, y, 0.f) - last_position;

    // TODO(@jparr721) - This sucks.
    Eigen::Vector3f dposition_scaled =
        (dposition.array() * Eigen::Array3f(0.01f, 0.01f, 0.0f)).matrix();

    // Click and drag with left moves top group
    if (event->buttons() & Qt::LeftButton) {
        mass_spring_system->TranslateTopGroup(dposition_scaled);
    }

    // Click and drag with right moved bottom group
    if (event->buttons() & Qt::RightButton) {
        mass_spring_system->TranslateBottomGroup(dposition_scaled);
    }

    last_position = Eigen::Vector3f(x, y, 0.f);
}
