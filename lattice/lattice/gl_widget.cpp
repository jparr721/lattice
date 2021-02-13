//
//  gl_widget.cpp
//  lattice
//
//  Created by Jarred Parr on 2/12/21.
//

#include "gl_widget.hpp"

#include <QtOpenGL/qgl.h>
#include <QtOpenGL/qglfunctions.h>

GLWidget::GLWidget(QWidget* parent) : QOpenGLWidget(parent) {
    // TODO(@jparr721) - Refactor to use smart pointer with .get()
    // Configure event handling with refresh rate
    draw_timer = new QTimer(this);
    connect(draw_timer, SIGNAL(timeout()), this, SLOT(updateGL()));

    // Time out every ~8 ms
    draw_timer->start(1000.0 / kBodyUpdatesPerSecond);

    // Configure update handling with update rate
    update_timer = new QTimer(this);
    connect(update_timer, SIGNAL(timeout()), this, SLOT(UpdateSim()));

    // Time out every ~33 ms
    update_timer->start(1000.0 / kRefreshRate);

    // Set up our change timer
    delta_timer.start();

    Initialize();
}

GLWidget::~GLWidget() {
    delete draw_timer;
    delete update_timer;
}

QSize GLWidget::minimumSizeHint()const { return QSize(100, 100); }

QSize GLWidget::sizeHint() const { return QSize(kWidth, kHeight); }

void GLWidget::initializeGL() {
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLWidget::resizeGL(int w, int h) {
    return;
}

// TODO(@jparr721) - Initialize mass spring system from spec
void GLWidget::Initialize() { return; }

void GLWidget::paintGL() { mass_spring_system->Render(); }

void GLWidget::UpdateSim() {
    float dt = delta_timer.elapsed();

    // Adapted from: https://gafferongames.com/post/fix_your_timestep/
    // Make sure the timestep doesn't get too large
    if (dt >= time_step_max) {
        dt = time_step_max;
    }

    delta_timer.restart();

    mass_spring_system->Update(dt);
}

void GLWidget::RestartSim() { Initialize(); }

void GLWidget::SetMass(float mass) {
    // TODO
}

void GLWidget::SetK(float stiffness) {
    // TODO
}

void GLWidget::SetDamping(float damping) {
    // TODO
}

void GLWidget::SetRestingLength(float length) {
    // TODO
}

void GLWidget::SetTimeStepSize(float step_size) {
    // TODO
}

void GLWidget::Grid() {
    // TODO
}
