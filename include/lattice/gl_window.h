#pragma once

#include <lattice/configuration.h>
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
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QSpinBox>

#include <memory>

class GLWidget;
class MainWindow;

class GLWindow : public QWidget {
    Q_OBJECT

  public:
    GLWidget* widget;

    explicit GLWindow(const MSSConfig& config);

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

    // DoubleSpinBox labels
    QLabel* mass_label{};
    QLabel* spring_constant_label{};
    QLabel* damping_constant_label{};
    QLabel* rest_length_label{};
    QLabel* time_step_label{};

    // All of our input objects in the mass spring system.
    QDoubleSpinBox* mass_input = new QDoubleSpinBox();
    QDoubleSpinBox* spring_constant_input = new QDoubleSpinBox();
    QDoubleSpinBox* damping_constant_input = new QDoubleSpinBox();
    QDoubleSpinBox* rest_length_input = new QDoubleSpinBox();
    QDoubleSpinBox* time_step_input = new QDoubleSpinBox();

    void CreateInputFields();
};
