#include "gl_widget.h"
#include "colors.h"
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

GLWidget::GLWidget(QWidget* parent) : QOpenGLWidget(parent) {
    setFocusPolicy(Qt::ClickFocus);

    InitializeSimulation();

    float updates_per_second = 120;
    float draws_per_second = 30;

    draw_timer = new QTimer(this);
    connect(draw_timer, &QTimer::timeout, this,
            QOverload<>::of(&GLWidget::update));
    draw_timer->start(1000.0 / draws_per_second);

    update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, this, &GLWidget::Update);
    update_timer->start(1000.0 / updates_per_second);

    delta_timer.start();
}

void GLWidget::Cleanup() { delete program_id; }

QSize GLWidget::minimumSizeHint() const { return QSize(kWidth, kHeight); }

QSize GLWidget::sizeHint() const { return QSize(kWidth, kHeight); }

void GLWidget::Update() {
    float dt = (float)delta_timer.elapsed() / 1000;

    delta_timer.restart();
    mass_spring_system->Update(dt);
}

void GLWidget::SetMass(float value) {
    slider_mass_value =
        Interpolate(kMinimumMassSliderValue, kMaximumMassSliderValue,
                    (float)value / 100.0f);

    mass_spring_system->SetMassWeight(slider_mass_value);

    emit OnMassChange(value);
    InitializeSimulation();
}

void GLWidget::SetSpringConstant(float value) {
    slider_spring_constant_value =
        Interpolate(kMinimumSpringConstantSliderValue,
                    kMaximumSpringConstantSliderValue, (float)value / 100.f);
    mass_spring_system->SetSpringStiffness(slider_spring_constant_value);
    emit OnSpringConstantChange(value);
    InitializeSimulation();
}

void GLWidget::SetSpringDampingConstant(float value) {
    slider_damping_constant_value =
        Interpolate(kMinimumDampingSliderValue, kMaximumDampingSliderValue,
                    (float)value / 100.f);
    mass_spring_system->SetMassDampingConstant(slider_damping_constant_value);
    emit OnSpringDampingChange(value);
    InitializeSimulation();
}

void GLWidget::SetSpringRestLength(float value) {
    slider_rest_length_value =
        Interpolate(kMinimumSpringRestLengthSliderValue,
                    kMaximumSpringRestLengthSliderValue, (float)value / 100.f);
    mass_spring_system->SetSpringRestLength(value);
    emit OnSpringRestLengthChange(value);
    InitializeSimulation();
}

void GLWidget::SetTimeStep(float value) {
    slider_time_step_value = value;
    emit OnTimeStepChange(value);
    InitializeSimulation();
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
    matrix.perspective(45.0f, 4.0f / 3.0f, 0.1f, 10.0f);
    matrix.translate(camera.x, camera.y, camera.z);

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

void GLWidget::keyPressEvent(QKeyEvent* event) {
    camera.OnKeyPress(event);
    int key = keyboard.OnKeyPressed(event->key());
    if (key > 0) {
        if (key == Keyboard::kPrint) {
            PrintParameters();
        } else if (key == Keyboard::kRestart) {
            InitializeSimulation();
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

void GLWidget::InitializeSimulation() {
    mass_spring_system = std::make_unique<MassSpringSystem>();
    mass_spring_system->Initialize();

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

QVector4D GLWidget::CurrentSimObjectVelocity() {
    return mass_spring_system->GetFirstMovingMassVelocity();
}

QVector4D GLWidget::CurrentSimObjectAcceleration() {
    return mass_spring_system->GetFirstMovingMassAcceleration();
}

QVector4D GLWidget::CurrentSimSpringForce() {
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
    last_position = QVector3D(event->x(), event->y(), 0.f);
}

void GLWidget::mouseMoveEvent(QMouseEvent* event) {
    int x = event->x();
    int y = event->y();
    QVector3D dposition = QVector3D(x, y, 0.f) - last_position;

    // TODO(@jparr721) - This sucks.
    QVector3D dposition_scaled = dposition * QVector3D(0.01f, 0.01f, 0.0f);

    // Click and drag with left moves top group
    if (event->buttons() & Qt::LeftButton) {
        mass_spring_system->TranslateTopGroup(dposition_scaled);
    }

    // Click and drag with right moved bottom group
    if (event->buttons() & Qt::RightButton) {
        mass_spring_system->TranslateBottomGroup(dposition_scaled);
    }

    last_position = QVector3D(x, y, 0.f);
}
