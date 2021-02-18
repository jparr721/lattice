#include "window.h"
#include "main_window.h"

#include <cstdlib>
#include <iostream>

#include <QGuiApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QSplineSeries>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QSlider>
#include <QStringLiteral>

Window::Window(MainWindow* _main_window) : main_window(_main_window) {
    CreateTimer();
    CreateChartsLayout();

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
    main_layout->addLayout(charts_layout);

    setLayout(main_layout);

    mass_slider->setValue(GLWidget::kMinimumMassSliderValue);
    spring_constant_slider->setValue(
        GLWidget::kMinimumSpringConstantSliderValue);
    damping_constant_slider->setValue(GLWidget::kMinimumDampingSliderValue);
    rest_length_slider->setValue(GLWidget::kMinimumSpringRestLengthSliderValue);
    time_step_slider->setValue(GLWidget::kMinimumTimeStepChangeSliderValue);

    setWindowTitle(tr(kWindowTitle));
    setFocus();
}

void Window::CreateChartsLayout() {
    force_y_line = new QtCharts::QSplineSeries(this);
    force_y_line->setName("Spring Force Y");

    force_chart = new QtCharts::QChart();
    force_chart->addSeries(force_y_line);
    force_chart->setTitle("Force Over Time");
    force_chart->addAxis(force_y_line_x_axis, Qt::AlignBottom);
    force_chart->addAxis(force_y_line_y_axis, Qt::AlignLeft);
    force_y_line->attachAxis(force_y_line_x_axis);
    force_y_line->attachAxis(force_y_line_y_axis);

    force_y_line_x_axis->setTickCount(5);
    force_y_line_x_axis->setRange(0, 25);
    force_y_line_y_axis->setRange(-10, 10);
    force_y_line_y_axis->setTickCount(5);

    force_chart_view = new QtCharts::QChartView(force_chart);
    // =============================================================

    /* acceleration_x_line = new QtCharts::QSplineSeries(this); */
    /* acceleration_x_line->setName("Acceleration Y Value"); */

    /* acceleration_chart = new QtCharts::QChart(); */
    /* acceleration_chart->addSeries(acceleration_x_line); */
    /* acceleration_chart->setTitle("Acceleration Over Time"); */
    /* acceleration_chart->addAxis(acceleration_y_line_x_axis, Qt::AlignBottom); */
    /* acceleration_chart->addAxis(acceleration_y_line_y_axis, Qt::AlignLeft); */
    /* acceleration_y_line->attachAxis(acceleration_y_line_x_axis); */
    /* acceleration_y_line->attachAxis(acceleration_y_line_y_axis); */

    /* acceleration_y_line_x_axis->setTickCount(5); */
    /* acceleration_y_line_x_axis->setRange(0, 25); */
    /* acceleration_y_line_y_axis->setTickCount(5); */
    /* acceleration_y_line_y_axis->setRange(-1000, 1000); */

    /* acceleration_chart_view = new QtCharts::QChartView(acceleration_chart); */

    // =============================================================

    charts_layout->addWidget(force_chart_view);
    /* charts_layout->addWidget(acceleration_chart_view); */
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
    connect(spring_constant_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetSpringConstant);
    connect(damping_constant_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetSpringDampingConstant);
    connect(rest_length_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetSpringRestLength);
    connect(time_step_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetTimeStep);
}

void Window::CreateTimer() {
    widget_poll_timeout = new QTimer(this);
    connect(widget_poll_timeout, &QTimer::timeout, this, &Window::UpdatePlots);
    widget_poll_timeout->start(100.0);
}

void Window::UpdatePlots() {
    UpdateForcePlot();
    /* UpdateAccelerationPlot(); */
    /* UpdateVelocityPlot(); */

    ++frame;
    if (frame == 25) {
        widget_poll_timeout->stop();
    }
}

void Window::UpdateForcePlot() {
    const auto current_force_vector = widget->CurrentSimSpringForce();
    force_y_line->append(frame, current_force_vector.y());
}

void Window::UpdateAccelerationPlot() {
    const auto current_acceleration_vector = widget->CurrentSimObjectAcceleration();
    acceleration_y_line->append(frame, current_acceleration_vector.y());
}

void Window::UpdateVelocityPlot() {
    const auto current_velocity_vector = widget->CurrentSimObjectVelocity();
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
