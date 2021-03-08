#pragma once

#include <lattice/camera.h>
#include <lattice/keyboard.h>
#include <lattice/shape_spec.h>
#include <lattice/supervisor.h>

#include <memory>
#include <string>

#include <QLineSeries>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QTimer>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
  public:
    GLWidget(std::shared_ptr<ShapeSpec> shape_spec, QWidget* parent = nullptr);
    ~GLWidget() = default;

    void RestartSimulation();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    Eigen::Vector3f CurrentSimObjectVelocity();
    Eigen::Vector3f CurrentSimObjectForce();
    Eigen::Vector3f CurrentSimSpringForce();

    bool IsRestarted();

  public slots:
    void Update();

    void SetMass(float value);
    void SetSpringConstant(float value);
    void SetSpringDampingConstant(float value);
    void SetSpringRestLength(float value);
    void SetTimeStep(float value);

    void Cleanup();

  signals:
    void OnMassChange(float value);
    void OnSpringConstantChange(float value);
    void OnSpringDampingChange(float value);
    void OnSpringRestLengthChange(float value);
    void OnTimeStepChange(float value);

  protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

  private:
    QOpenGLShaderProgram* program_id = nullptr;

    // Window Parameters
    constexpr static int kWidth = 640;
    constexpr static int kHeight = 720;

    // Positional Translation Parameters
    Eigen::Vector3f last_position = Eigen::Vector3f::Zero();

    // Slider Data
    float slider_mass_value = Mass::kMinimumMassValue;
    float slider_spring_constant_value = Spring::kMinimumSpringConstantValue;
    float slider_damping_constant_value = Spring::kMinimumDampingValue;
    float slider_rest_length_value = Spring::kMinimumSpringRestLengthValue;
    float slider_time_step_value =
        MassSpringSystem::kMinimumTimeStepChangeValue;

    // Draw Timers for the sim time step
    QTimer* draw_timer;
    QTimer* update_timer;

    // Simulation Params
    std::shared_ptr<Supervisor> supervisor;

    // Camera Controller
    Camera camera;

    // Keyboard Controller
    Keyboard keyboard;

    // Other misc params
    GLint position = 0;
    GLint color = 0;
    GLint matrix_uniform = 0;
    int frame = 0;

    bool is_init = false;
    bool is_restarted = false;

    std::string ReadVertexShader();
    std::string ReadFragmentShader();

    float Interpolate(float v0, float v1, float t);

    void PrintParameters();
};
