#pragma once

#include <lattice/camera.h>
#include <lattice/configuration.h>
#include <lattice/keyboard.h>
#include <lattice/stats.h>
#include <lattice/supervisor.h>

#include <memory>
#include <string>

#include <QLineSeries>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QThread>
#include <QTimer>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
  public:
    // Simulation Params
    std::shared_ptr<Supervisor> supervisor;

    explicit GLWidget(MSSConfig config, QWidget* parent = nullptr);
    ~GLWidget() override;

    void RestartSimulation() const;

    [[nodiscard]] QSize minimumSizeHint() const override;
    [[nodiscard]] QSize sizeHint() const override;

  public slots:
    void Update() const;
    void SaveCurrentStats();

    void Permute();

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
    constexpr static int kWidth = 640;
    constexpr static int kHeight = 720;

    // 10 Second Permute Timeout Interval
    constexpr static int kPermutationTimeout = 10000;

    // Draw Timers for the sim time step
    QTimer* draw_timer;
    QTimer* update_timer;

    // Simulation Timer For Object Permutations
    QTimer* permutation_timeout;

    // Camera Controller
    Camera camera;

    // Keyboard Controller
    Keyboard keyboard;

    // Stats Controller
    Stats* stats;

    // Stats Worker Thread
    QThread worker_thread;

    // Internal Config Object
    MSSConfig config;

    // Other misc params
    GLint position = 0;
    GLint color = 0;
    GLint matrix_uniform = 0;
    int frame = 0;

    bool is_init = false;

    static std::string ReadVertexShader();
    static std::string ReadFragmentShader();

    static float Interpolate(float v0, float v1, float t);

    void PrintParameters() const;
};
