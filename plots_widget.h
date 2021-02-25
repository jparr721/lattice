#pragma once

#include <QWidget>
#include <QChart>
#include <QChartView>
#include <QHBoxLayout>
#include <QSplineSeries>
#include <QTimer>
#include <QValueAxis>
#include <QScrollArea>

class Window;

class PlotsWidget : public QWidget {
    Q_OBJECT

  public:
      explicit PlotsWidget(Window* _window, QWidget* parent = nullptr);

  public slots:
    void UpdatePlots();
    void ResetPlots();

  private:
    constexpr static int kMinFrame = 0;
    constexpr static int kMaxFrame = 1000;
    int frame = 0;

    Window* window;

    // Layouts - Plots
    QHBoxLayout* charts_layout = new QHBoxLayout;

    // Force Line Chart Axes
    QtCharts::QValueAxis* force_y_line_x_axis = new QtCharts::QValueAxis();
    QtCharts::QValueAxis* force_y_line_y_axis = new QtCharts::QValueAxis();

    // Line chart for spring force data
    QtCharts::QSplineSeries* force_z_line;
    QtCharts::QSplineSeries* force_y_line;

    QtCharts::QChart* force_chart;
    QtCharts::QChartView* force_chart_view;

    // Acceleration Line Chart Axes
    QtCharts::QValueAxis* acceleration_y_line_x_axis =
        new QtCharts::QValueAxis();
    QtCharts::QValueAxis* acceleration_y_line_y_axis =
        new QtCharts::QValueAxis();

    // Line chart for mass accleration data
    QtCharts::QSplineSeries* acceleration_x_line;
    QtCharts::QSplineSeries* acceleration_y_line;

    QtCharts::QChart* acceleration_chart;
    QtCharts::QChartView* acceleration_chart_view;

    // Velocity Line Chart Axes
    QtCharts::QValueAxis* velocity_y_line_x_axis = new QtCharts::QValueAxis();
    QtCharts::QValueAxis* velocity_y_line_y_axis = new QtCharts::QValueAxis();

    // Line chart for mass accleration data
    QtCharts::QSplineSeries* velocity_x_line;
    QtCharts::QSplineSeries* velocity_y_line;

    QtCharts::QChart* velocity_chart;
    QtCharts::QChartView* velocity_chart_view;

    // Time for polling glWidget for the latest force values
    QTimer* widget_poll_timeout;

    // Time for polling glWidget for reset status for updating plots
    QTimer* plot_poll_timeout;

    void CreateTimer();

    // Plot Reset
    void ResetForcePlot();
    void ResetAccelerationPlot();
    void ResetVelocityPlot();

    // Plot Updates
    void UpdateForcePlot();
    void UpdateAccelerationPlot();
    void UpdateVelocityPlot();
};
