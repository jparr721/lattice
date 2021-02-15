#pragma once

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

  protected:
    void keyPressEvent(QKeyEvent* event) override;

  private slots:
    void EndSimulation();

  private:
    constexpr static const char* kWindowTitle = "Lattice Sim";

    GLWidget* widget;

    // All of our slider objects in the mass spring system.
    QSlider* mass_slider;
    QSlider* spring_constant_slider;
    QSlider* damping_constant_slider;
    QSlider* rest_length_slider;
    QSlider* time_step_slider;

    // This button ends the simulation early.
    QPushButton* end_sim_button;

    // A reference to our management window instance.
    MainWindow* main_window;

    QSlider* CreateSlider();
};