#include <lattice/gl_widget.h>
#include <lattice/gl_window.h>

#include <fstream>

#include <QOpenGLShaderProgram>

GLWidget::GLWidget(MSSConfig config, QWidget* parent)
    : config(config), QOpenGLWidget(parent) {
    setFocusPolicy(Qt::ClickFocus);

    const auto initial_parameters =
        SupervisorParameters{.mass = 0.5,
                             .k = 50,
                             .damping = 0.5,
                             .rest_length = 2,
                             .time_step = .005095};

    supervisor = std::make_shared<Supervisor>(config, initial_parameters);

    constexpr int msec_conversion = 1000;
    constexpr int updates_per_second = 120;
    constexpr int draws_per_second = 30;

    permutation_timeout = new QTimer(this);
    connect(permutation_timeout, &QTimer::timeout, this, &GLWidget::Permute);
    permutation_timeout->start(kPermutationTimeout);

    draw_timer = new QTimer(this);
    connect(draw_timer, &QTimer::timeout, this,
            QOverload<>::of(&GLWidget::update));
    draw_timer->start(msec_conversion / draws_per_second);

    update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, this, &GLWidget::Update);
    update_timer->start(msec_conversion / updates_per_second);

    // Configure background stats thread
    stats = new Stats(supervisor);
    stats->moveToThread(&worker_thread);
    connect(&worker_thread, &QThread::finished, stats, &QObject::deleteLater);
    connect(update_timer, &QTimer::timeout, this, &GLWidget::SaveCurrentStats);
    worker_thread.start();
}

GLWidget::~GLWidget() {
    worker_thread.quit();
    worker_thread.wait();
}

void GLWidget::Cleanup() { delete program_id; }

QSize GLWidget::minimumSizeHint() const { return QSize(kWidth, kHeight); }

QSize GLWidget::sizeHint() const { return QSize(kWidth, kHeight); }

void GLWidget::SaveCurrentStats() { stats->DropReading(); }

void GLWidget::Update() const { supervisor->Update(); }

void GLWidget::SetMass(float value) {
    supervisor->SetMassWeight(value);
    emit OnMassChange(value);
}

void GLWidget::SetSpringConstant(float value) {
    supervisor->SetSpringConstant(value);
    emit OnSpringConstantChange(value);
}

void GLWidget::SetSpringDampingConstant(float value) {
    supervisor->SetSpringDampingConstant(value);
    emit OnSpringDampingChange(value);
}

void GLWidget::SetSpringRestLength(float value) {
    supervisor->SetSpringRestLength(value);
    emit OnSpringRestLengthChange(value);
}

void GLWidget::SetTimeStep(float value) {
    supervisor->SetTimeStep(value);
    emit OnTimeStepChange(value);
}

void GLWidget::initializeGL() {
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this,
            &GLWidget::Cleanup);
    initializeOpenGLFunctions();

    const auto vertex_shader = ReadVertexShader();
    const auto fragment_shader = ReadFragmentShader();

    program_id = new QOpenGLShaderProgram(this);
    program_id->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                        vertex_shader.data());
    program_id->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                        fragment_shader.data());
    program_id->link();
    position = program_id->attributeLocation("position_attribute");
    Q_ASSERT(position != -1);
    color = program_id->attributeLocation("color_attribute");
    Q_ASSERT(color != -1);
    matrix_uniform = program_id->uniformLocation("projection_matrix");
    Q_ASSERT(matrix_uniform != -1);

    is_init = true;
}

void GLWidget::paintGL() {
    assert(is_init && "Not Initialized");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program_id->bind();
    QMatrix4x4 matrix;
    matrix.perspective(45.0f, 4.0f / 3.0f, 0.1f, 200.0f);
    matrix.translate(camera.x, camera.y, camera.z);

    // TODO(@jparr721) - This kinda sucks.. I'll make this translate the shape
    // later instead.
    matrix.rotate(camera.rot, camera.direction);

    program_id->setUniformValue(matrix_uniform, matrix);
    auto shapes = supervisor->shapes;
    auto colors = supervisor->colors;

    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0,
                          static_cast<void*>(shapes.data()));
    glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 0,
                          static_cast<void*>(colors.data()));

    glEnableVertexAttribArray(position);
    glEnableVertexAttribArray(color);

    glDrawArrays(GL_TRIANGLES, 0, shapes.size());

    glDisableVertexAttribArray(color);
    glDisableVertexAttribArray(position);

    program_id->release();
    ++frame;
}

// Don't allow resizing for now.
void GLWidget::resizeGL(int width, int height) {}

void GLWidget::keyPressEvent(QKeyEvent* event) {
    camera.OnKeyPress(event);
    const int key = keyboard.OnKeyPressed(event->key());
    if (key > 0) {
        if (key == Keyboard::kPrint) {
            PrintParameters();
        } else if (key == Keyboard::kRestart) {
            RestartSimulation();
        }
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent* event) {
    keyboard.OnKeyReleased(event->key());
}

std::string GLWidget::ReadVertexShader() {
    std::ostringstream sstr;
    auto stream = std::ifstream{"../../shaders/core.vs"};
    sstr << stream.rdbuf();
    return sstr.str();
}

std::string GLWidget::ReadFragmentShader() {
    std::ostringstream sstr;
    auto stream = std::ifstream{"../../shaders/core.frag"};
    sstr << stream.rdbuf();
    return sstr.str();
}

float GLWidget::Interpolate(float v0, float v1, float t) {
    return v0 + t * (v1 - v0);
}

void GLWidget::RestartSimulation() {
    supervisor->Reset();
    emit OnMassChange(supervisor->GetMassWeight());
    emit OnSpringConstantChange(supervisor->GetSpringConstant());
    emit OnSpringDampingChange(supervisor->GetSpringDampingConstant());
    emit OnSpringRestLengthChange(supervisor->GetSpringRestLength());
    emit OnTimeStepChange(supervisor->GetTimeStep());
}

void GLWidget::Permute() {
    auto current_parameters = supervisor->CurrentParameters();

    if (supervisor->sine_step == supervisor->kMaxSineStep) {
        config.Permute();
        supervisor->sine_step = 0;
    } else {
        current_parameters.k = supervisor->ComputeNextK();
    }

    supervisor->ReInitialize(config, current_parameters);
    emit OnSpringConstantChange(supervisor->GetSpringConstant());
}