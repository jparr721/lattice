//
//  gl_widget.hpp
//  lattice
//
//  Created by Jarred Parr on 2/12/21.
//

#ifndef gl_widget_hpp
#define gl_widget_hpp

#include <QtCore/qelapsedtimer.h>
#include <QtCore/qmetaobject.h>
#include <QtCore/qobject.h>
#include <QtCore/qtimer.h>
#include <QtOpenGL/qgl.h>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qopenglwidget.h>
#include <cmath>
#include <memory>

#include "mass_spring_system.hpp"

class GLWidget : public QOpenGLWidget {
    Q_OBJECT
  public:
    GLWidget(QWidget* parent = 0);
    ~GLWidget();

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

  protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;

  public slots:
    void paintGL();
    void UpdateSim();
    void RestartSim();

    // ===================
    // Object Manipulation:
    void SetMass(float mass);
    void SetK(float stiffness);
    void SetDamping(float damping);
    void SetRestingLength(float length);
    void SetTimeStepSize(float step_size);

  signals:
    void OnMassChange(float mass);
    void OnKChange(float stiffness);
    void OnDampingChange(float damping);
    void OnRestLengthChange(float length);
    void OnTimeStepChange(float step_size);

  private:
    // Window width
    constexpr static int kWidth = 1300;

    // Window height
    constexpr static int kHeight = 900;

    // Update Rate for masses and springs
    constexpr static float kBodyUpdatesPerSecond = 120.f;

    // Refresh Rate for drawing masses and springs
    constexpr static float kRefreshRate = 30; // 30 frames per second.

    // The Mass Spring System Simulator
    std::shared_ptr<MassSpringSystem> mass_spring_system;

    // Simulation Timers
    QTimer* draw_timer;
    QTimer* update_timer;
    QElapsedTimer delta_timer;

    // Render Status
    bool is_rendering = false;

    // Initialization Status
    bool is_init = false;

    // Slider Variables
    float slider_mass;
    float slider_k;
    float slider_damping;
    float slider_rest_length;
    float slider_time_step;

    // Slider Variable Min Values
    constexpr static float slider_mass_min = 0.05;
    constexpr static float slider_k_min = 1.0;
    constexpr static float slider_damping_min = 0.05;
    constexpr static float slider_rest_length_min = 0.05;

    // Slider Variable Max Values
    constexpr static float slider_mass_max = 1.f;
    constexpr static float slider_k_max = 50.f;
    constexpr static float slider_damping_max = 5.f;
    constexpr static float slider_rest_length_max = 1.0f;

    // Time step variables
    const float time_step_min = std::floor(1000.0 / 320.f);
    const float time_step_max = std::floor(1000.0 / 24.f);

    void Grid();
    void Initialize();
};

#endif /* gl_widget_hpp */
