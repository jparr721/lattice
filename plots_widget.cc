#include "plots_widget.h"
#include "gl_window.h"

#include <iostream>

PlotsWidget::PlotsWidget(GLWindow* _window, QWidget* parent)
    : window(_window), QWidget(parent) {
    CreateTimer();

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
    force_y_line_y_axis->setRange(0, 200);
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
    acceleration_y_line_y_axis->setRange(-50, 50);

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
    velocity_y_line_y_axis->setRange(-50, 50);

    velocity_chart_view = new QtCharts::QChartView(velocity_chart);
    // =============================================================

    charts_layout->addWidget(force_chart_view);
    charts_layout->addWidget(acceleration_chart_view);
    charts_layout->addWidget(velocity_chart_view);

    setLayout(charts_layout);
}

void PlotsWidget::CreateTimer() {
    widget_poll_timeout = new QTimer(this);
    connect(widget_poll_timeout, &QTimer::timeout, this,
            &PlotsWidget::UpdatePlots);
    widget_poll_timeout->start(1);

    plot_poll_timeout = new QTimer(this);
    connect(plot_poll_timeout, &QTimer::timeout, this,
            &PlotsWidget::ResetPlots);
    plot_poll_timeout->start(1);
}

void PlotsWidget::UpdatePlots() {
    UpdateForcePlot();
    UpdateAccelerationPlot();
    UpdateVelocityPlot();

    if (frame == kMaxFrame) {
        widget_poll_timeout->stop();
    }

    ++frame;
}

void PlotsWidget::ResetPlots() {
    if (window->widget->IsRestarted()) {
        // If we reach the frame threshold, we need to restart the timer
        if (!widget_poll_timeout->isActive()) {
            widget_poll_timeout->start();
        }

        ResetForcePlot();
        ResetAccelerationPlot();
        ResetVelocityPlot();

        frame = 0;
    }
}

void PlotsWidget::ResetForcePlot() { force_y_line->clear(); }

void PlotsWidget::UpdateForcePlot() {
    const auto current_force_vector = window->widget->CurrentSimSpringForce();
    force_y_line->append(frame, current_force_vector.y());
}

void PlotsWidget::ResetAccelerationPlot() { acceleration_y_line->clear(); }

void PlotsWidget::UpdateAccelerationPlot() {
    const auto current_acceleration_vector =
        window->widget->CurrentSimObjectForce();
    acceleration_y_line->append(frame, current_acceleration_vector.y());
}

void PlotsWidget::ResetVelocityPlot() { velocity_y_line->clear(); }

void PlotsWidget::UpdateVelocityPlot() {
    const auto current_velocity_vector =
        window->widget->CurrentSimObjectVelocity();
    velocity_y_line->append(frame, current_velocity_vector.y());
}
