#pragma once

#include <lattice/generator.h>
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

class GLWindow : public QWidget {
    Q_OBJECT

  public:
    GLWidget* widget;

    explicit GLWindow(const generator::MSSConfig& config);

  protected:
    void keyPressEvent(QKeyEvent* event) override;

  private:
    constexpr static const char* kWindowTitle = "Lattice Sim";

    // Layouts - Main
    QVBoxLayout* main_layout = new QVBoxLayout;

    // Layouts - Simulation
    QHBoxLayout* sim_layout = new QHBoxLayout;

    // Layouts - Controls
    QVBoxLayout* controls_layout = new QVBoxLayout;

    // Slider labels
    QLabel* mass_label{};
    QLabel* spring_constant_label{};
    QLabel* damping_constant_label{};
    QLabel* rest_length_label{};
    QLabel* time_step_label{};

    // All of our slider objects in the mass spring system.
    QSlider* mass_slider{};
    QSlider* spring_constant_slider{};
    QSlider* damping_constant_slider{};
    QSlider* rest_length_slider{};
    QSlider* time_step_slider{};

    static QSlider* CreateSlider();

    void CreateSliders();
};
