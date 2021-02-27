#include "gl_window.h"
#include "main_window.h"

#include <cstdlib>
#include <iostream>

#include <QGuiApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QSlider>
#include <QSplineSeries>
#include <QStringLiteral>

GLWindow::GLWindow() {
    CreateSliders();

    sim_layout->addWidget(widget);

    /** Mass Label and Slider **/
    controls_layout->addWidget(mass_label, 0, Qt::AlignTop);
    controls_layout->addWidget(mass_slider, 0, Qt::AlignTop);

    /** Spring Label and Slider **/
    controls_layout->addWidget(spring_constant_label, 0, Qt::AlignTop);
    controls_layout->addWidget(spring_constant_slider, 0, Qt::AlignTop);

    /** Damping Label and Slider **/
    controls_layout->addWidget(damping_constant_label, 0, Qt::AlignTop);
    controls_layout->addWidget(damping_constant_slider, 0, Qt::AlignTop);

    /** Rest Length Label and Slider **/
    controls_layout->addWidget(rest_length_label, 0, Qt::AlignTop);
    controls_layout->addWidget(rest_length_slider, 0, Qt::AlignTop);

    /** Time Step Label and Slider **/
    controls_layout->addWidget(time_step_label, 0, Qt::AlignTop);
    controls_layout->addWidget(time_step_slider, 0, Qt::AlignTop);

    sim_layout->addLayout(controls_layout);

    sim_layout->setStretchFactor(widget, 2);
    sim_layout->setStretchFactor(controls_layout, 1);

    main_layout->addLayout(sim_layout);

    setLayout(main_layout);

    mass_slider->setValue(Mass::kMinimumMassValue);
    spring_constant_slider->setValue(Spring::kMinimumSpringConstantValue);
    damping_constant_slider->setValue(Spring::kMinimumDampingValue);
    rest_length_slider->setValue(Spring::kMinimumSpringRestLengthValue);
    time_step_slider->setValue(MassSpringSystem::kMinimumTimeStepChangeValue);

    setWindowTitle(tr(kWindowTitle));
    setFocus();
}

void GLWindow::CreateSliders() {
    mass_label = new QLabel("Mass");
    mass_slider = CreateSlider();

    spring_constant_label = new QLabel("K");
    spring_constant_slider = CreateSlider();

    damping_constant_label = new QLabel("Damping");
    damping_constant_slider = CreateSlider();

    rest_length_label = new QLabel("Rest Length");
    rest_length_slider = CreateSlider();

    time_step_label = new QLabel("Time Step");
    time_step_slider = CreateSlider();

    connect(mass_slider, &QSlider::valueChanged, widget, &GLWidget::SetMass);
    connect(widget, &GLWidget::OnMassChange, mass_slider, &QSlider::setValue);

    connect(spring_constant_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetSpringConstant);
    connect(widget, &GLWidget::OnSpringConstantChange, spring_constant_slider,
            &QSlider::setValue);

    connect(damping_constant_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetSpringDampingConstant);
    connect(widget, &GLWidget::OnSpringDampingChange, damping_constant_slider,
            &QSlider::setValue);

    connect(rest_length_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetSpringRestLength);
    connect(widget, &GLWidget::OnSpringRestLengthChange, rest_length_slider,
            &QSlider::setValue);

    connect(time_step_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetTimeStep);
    connect(widget, &GLWidget::OnTimeStepChange, time_step_slider,
            &QSlider::setValue);
}

QSlider* GLWindow::CreateSlider() {
    auto slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setSingleStep(5);
    slider->setPageStep(5);
    slider->setTickInterval(5);
    slider->setTickPosition(QSlider::TicksRight);

    return slider;
}

void GLWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    } else {
        QWidget::keyPressEvent(event);
    }
}
