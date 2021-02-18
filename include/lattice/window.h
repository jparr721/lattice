#pragma once

#include <lattice/gl_widget.h>

#include <QChart>
#include <QChartView>
#include <QHBoxLayout>
#include <QLabel>
#include <QSplineSeries>
#include <QTimer>
#include <QVBoxLayout>
#include <QValueAxis>
#include <QWidget>

#include <memory>

QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
QT_END_NAMESPACE

class GLWidget;
class MainWindow;

class Window : public QWidget {
    Q_OBJECT

  public:
    Window(MainWindow* _main_window);

  public slots:
    void UpdatePlots();

  protected:
    void keyPressEvent(QKeyEvent* event) override;

  private:
    constexpr static int kMinFrame = 0;
    constexpr static int kMaxFrame = 250;
    constexpr static const char* kWindowTitle = "Lattice Sim";

    GLWidget* widget = new GLWidget;

    // Layouts - Main
    QVBoxLayout* main_layout = new QVBoxLayout;

    // Layouts - Plots
    QHBoxLayout* charts_layout = new QHBoxLayout;

    // Layouts - Simulation
    QHBoxLayout* sim_layout = new QHBoxLayout;

    // Layouts - Controls
    QVBoxLayout* controls_layout = new QVBoxLayout;

    // Slider labels
    QLabel* mass_label;
    QLabel* spring_constant_label;
    QLabel* damping_constant_label;
    QLabel* rest_length_label;
    QLabel* time_step_label;

    // All of our slider objects in the mass spring system.
    QSlider* mass_slider;
    QSlider* spring_constant_slider;
    QSlider* damping_constant_slider;
    QSlider* rest_length_slider;
    QSlider* time_step_slider;

    // Force Line Chart Axes
    QtCharts::QValueAxis* force_y_line_x_axis = new QtCharts::QValueAxis();
    QtCharts::QValueAxis* force_y_line_y_axis = new QtCharts::QValueAxis();

    // Line chart for spring force data
    QtCharts::QSplineSeries* force_z_line;
    QtCharts::QSplineSeries* force_y_line;

    QtCharts::QChart* force_chart;
    QtCharts::QChartView* force_chart_view;

    // Acceleration Line Chart Axes
    QtCharts::QValueAxis* acceleration_y_line_x_axis = new QtCharts::QValueAxis();
    QtCharts::QValueAxis* acceleration_y_line_y_axis = new QtCharts::QValueAxis();

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

    // A reference to our management window instance.
    MainWindow* main_window;

    int frame = kMinFrame;

    QSlider* CreateSlider();

    void CreateTimer();

    void CreateSliders();

    void CreateChartsLayout();

    void CreateSimLayout();

    void UpdateForcePlot();
    void UpdateAccelerationPlot();
    void UpdateVelocityPlot();

};
