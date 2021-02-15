#pragma once

#include "mass_spring_system.h"

#include <memory>
#include <string>

#include <QElapsedTimer>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QTimer>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
  public:
    // Slider Minimum Values
    constexpr static float kMinimumMassSliderValue = 0.5f;
    constexpr static float kMinimumSpringConstantSliderValue = 1.0f;
    constexpr static float kMinimumDampingSliderValue = 0.5;
    constexpr static float kMinimumSpringRestLengthSliderValue = 0.5f;
    constexpr static float kMinimumTimeStepChangeSliderValue = 0.0001;

    // Slider Maxmum Values
    constexpr static float kMaximumMassSliderValue = 10.0f;
    constexpr static float kMaximumSpringConstantSliderValue = 50.0f;
    constexpr static float kMaximumDampingSliderValue = 5.0f;
    constexpr static float kMaximumSpringRestLengthSliderValue = 10.0f;
    constexpr static float kMaximumTimeStepChangeSliderValue = 1.0f;

    explicit GLWidget(QWidget* parent = nullptr);
    ~GLWidget() = default;

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

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

  private:
    QOpenGLShaderProgram* program_id = nullptr;

    // Window Parameters
    constexpr static int kWidth = 1200;
    constexpr static int kHeight = 1000;

    // Slider Data
    float slider_mass_value = kMinimumMassSliderValue;
    float slider_spring_constant_value = kMinimumSpringConstantSliderValue;
    float slider_damping_constant_value = kMinimumDampingSliderValue;
    float slider_rest_length_value = kMinimumSpringRestLengthSliderValue;
    float slider_time_step_value = kMinimumTimeStepChangeSliderValue;

    // Draw Timers for the sim time step
    QTimer* draw_timer;
    QTimer* update_timer;
    QElapsedTimer delta_timer;

    // Simulation Params
    std::shared_ptr<MassSpringSystem> mass_spring_system;

    // Other misc params
    GLint position = 0;
    GLint color = 0;
    GLint matrix_uniform = 0;
    int frame = 0;

    bool is_init = false;

    std::string ReadVertexShader();
    std::string ReadFragmentShader();
};