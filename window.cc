#include "window.h"
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

Window::Window(MainWindow* _main_window) : main_window(_main_window) {
    CreateChartsLayout();

    CreateSliders();

    CreateTimer();

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
    main_layout->addLayout(charts_layout);

    setLayout(main_layout);

    mass_slider->setValue(MassSpringSystem::kMinimumMassValue);
    spring_constant_slider->setValue(
        MassSpringSystem::kMinimumSpringConstantValue);
    damping_constant_slider->setValue(MassSpringSystem::kMinimumDampingValue);
    rest_length_slider->setValue(
        MassSpringSystem::kMinimumSpringRestLengthValue);
    time_step_slider->setValue(MassSpringSystem::kMinimumTimeStepChangeValue);

    setWindowTitle(tr(kWindowTitle));
    setFocus();
}

void Window::CreateChartsLayout() {
    force_y_line = new QtCharts::QSplineSeries(this);
    force_y_line->setName("Spring Force Y Value");

    force_chart = new QtCharts::QChart();
    force_chart->addSeries(force_y_line);
    force_chart->setTitle("Force Over Time");
    force_chart->addAxis(force_y_line_x_axis, Qt::AlignBottom);
    force_chart->addAxis(force_y_line_y_axis, Qt::AlignLeft);
    force_y_line->attachAxis(force_y_line_x_axis);
    force_y_line->attachAxis(force_y_line_y_axis);

    force_y_line_x_axis->setTickCount(5);
    force_y_line_x_axis->setRange(kMinFrame, kMaxFrame);
    force_y_line_y_axis->setRange(0, 100);
    force_y_line_y_axis->setTickCount(5);

    force_chart_view = new QtCharts::QChartView(force_chart);
    // =============================================================

    acceleration_y_line = new QtCharts::QSplineSeries(this);
    acceleration_y_line->setName("Acceleration Y Value");

    acceleration_chart = new QtCharts::QChart();
    acceleration_chart->addSeries(acceleration_y_line);
    acceleration_chart->setTitle("Acceleration Over Time");
    acceleration_chart->addAxis(acceleration_y_line_x_axis, Qt::AlignBottom);
    acceleration_chart->addAxis(acceleration_y_line_y_axis, Qt::AlignLeft);

    acceleration_y_line->attachAxis(acceleration_y_line_x_axis);
    acceleration_y_line->attachAxis(acceleration_y_line_y_axis);

    acceleration_y_line_x_axis->setTickCount(5);
    acceleration_y_line_x_axis->setRange(kMinFrame, kMaxFrame);
    acceleration_y_line_y_axis->setTickCount(5);
    acceleration_y_line_y_axis->setRange(-10, 10);

    acceleration_chart_view = new QtCharts::QChartView(acceleration_chart);

    // =============================================================
    velocity_y_line = new QtCharts::QSplineSeries(this);
    velocity_y_line->setName("Velocity Y Value");

    velocity_chart = new QtCharts::QChart();
    velocity_chart->addSeries(velocity_y_line);
    velocity_chart->setTitle("Velocity Over Time");
    velocity_chart->addAxis(velocity_y_line_x_axis, Qt::AlignBottom);
    velocity_chart->addAxis(velocity_y_line_y_axis, Qt::AlignLeft);

    velocity_y_line->attachAxis(velocity_y_line_x_axis);
    velocity_y_line->attachAxis(velocity_y_line_y_axis);

    velocity_y_line_x_axis->setTickCount(5);
    velocity_y_line_x_axis->setRange(kMinFrame, kMaxFrame);
    velocity_y_line_y_axis->setTickCount(5);
    velocity_y_line_y_axis->setRange(-10, 10);

    velocity_chart_view = new QtCharts::QChartView(velocity_chart);
    // =============================================================

    charts_layout->addWidget(force_chart_view);
    charts_layout->addWidget(acceleration_chart_view);
    charts_layout->addWidget(velocity_chart_view);
}

void Window::CreateSliders() {
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
    connect(widget, &GLWidget::OnSpringConstantChange, spring_constant_slider, &QSlider::setValue);

    connect(damping_constant_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetSpringDampingConstant);
    connect(widget, &GLWidget::OnSpringDampingChange, damping_constant_slider, &QSlider::setValue);

    connect(rest_length_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetSpringRestLength);
    connect(widget, &GLWidget::OnSpringRestLengthChange, rest_length_slider, &QSlider::setValue);

    connect(time_step_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetTimeStep);
    connect(widget, &GLWidget::OnTimeStepChange, time_step_slider, &QSlider::setValue);
}

void Window::CreateTimer() {
    widget_poll_timeout = new QTimer(this);
    connect(widget_poll_timeout, &QTimer::timeout, this, &Window::UpdatePlots);
    widget_poll_timeout->start(1);

    plot_poll_timeout = new QTimer(this);
    connect(plot_poll_timeout, &QTimer::timeout, this, &Window::ResetPlots);
    plot_poll_timeout->start(1);
}

void Window::UpdatePlots() {
    UpdateForcePlot();
    UpdateAccelerationPlot();
    UpdateVelocityPlot();

    if (frame == kMaxFrame) {
        widget_poll_timeout->stop();
    }
    ++frame;
}

void Window::ResetPlots() {
    if (widget->IsRestarted()) {
        ResetForcePlot();
        ResetAccelerationPlot();
        ResetVelocityPlot();

        frame = 0;
    }
}

void Window::ResetForcePlot() { force_y_line->clear(); }

void Window::UpdateForcePlot() {
    const auto current_force_vector = widget->CurrentSimSpringForce();
    force_y_line->append(frame, current_force_vector.y());
}

void Window::ResetAccelerationPlot() { acceleration_y_line->clear(); }

void Window::UpdateAccelerationPlot() {
    const auto current_acceleration_vector =
        widget->CurrentSimObjectAcceleration();
    acceleration_y_line->append(frame, current_acceleration_vector.y());
}

void Window::ResetVelocityPlot() { velocity_y_line->clear(); }

void Window::UpdateVelocityPlot() {
    const auto current_velocity_vector = widget->CurrentSimObjectVelocity();
    velocity_y_line->append(frame, current_velocity_vector.y());
}

QSlider* Window::CreateSlider() {
    auto slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setSingleStep(5);
    slider->setPageStep(5);
    slider->setTickInterval(5);
    slider->setTickPosition(QSlider::TicksRight);

    return slider;
}

void Window::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    } else {
        QWidget::keyPressEvent(event);
    }
}
