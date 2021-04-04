#include <lattice/gl_window.h>
#include <lattice/toolbox/numerics.h>

#include <QString>

GLWindow::GLWindow(const MSSConfig& config) {
    widget = new GLWidget(config);

    CreateInputFields();

    sim_layout->addWidget(widget);

    /** Mass Label and DoubleSpinBox **/
    controls_layout->addWidget(mass_label, 0, Qt::AlignTop);
    controls_layout->addWidget(mass_input, 0, Qt::AlignTop);

    /** Spring Label and DoubleSpinBox **/
    controls_layout->addWidget(spring_constant_label, 0, Qt::AlignTop);
    controls_layout->addWidget(spring_constant_input, 0, Qt::AlignTop);

    /** Damping Label and DoubleSpinBox **/
    controls_layout->addWidget(damping_constant_label, 0, Qt::AlignTop);
    controls_layout->addWidget(damping_constant_input, 0, Qt::AlignTop);

    /** Rest Length Label and DoubleSpinBox **/
    controls_layout->addWidget(rest_length_label, 0, Qt::AlignTop);
    controls_layout->addWidget(rest_length_input, 0, Qt::AlignTop);

    /** Time Step Label and DoubleSpinBox **/
    controls_layout->addWidget(time_step_label, 0, Qt::AlignTop);
    controls_layout->addWidget(time_step_input, 0, Qt::AlignTop);

    sim_layout->addLayout(controls_layout);

    sim_layout->setStretchFactor(widget, 2);
    sim_layout->setStretchFactor(controls_layout, 1);

    main_layout->addLayout(sim_layout);

    setLayout(main_layout);

    mass_input->setValue(widget->supervisor->GetMassWeight());
    spring_constant_input->setValue(widget->supervisor->GetSpringConstant());
    damping_constant_input->setValue(widget->supervisor->GetSpringDampingConstant());
    rest_length_input->setValue(widget->supervisor->GetSpringRestLength());
    time_step_input->setValue(widget->supervisor->GetTimeStep());

    connect(mass_input, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            widget, &GLWidget::SetMass);
    connect(spring_constant_input,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged), widget,
            &GLWidget::SetSpringConstant);
    connect(damping_constant_input,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged), widget,
            &GLWidget::SetSpringDampingConstant);
    connect(rest_length_input,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged), widget,
            &GLWidget::SetSpringRestLength);
    connect(time_step_input,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged), widget,
            &GLWidget::SetTimeStep);

    connect(widget, &GLWidget::OnMassChange, mass_input,
            &QDoubleSpinBox::setValue);
    connect(widget, &GLWidget::OnSpringConstantChange, spring_constant_input,
            &QDoubleSpinBox::setValue);
    connect(widget, &GLWidget::OnSpringDampingChange, damping_constant_input,
            &QDoubleSpinBox::setValue);
    connect(widget, &GLWidget::OnSpringRestLengthChange, rest_length_input,
            &QDoubleSpinBox::setValue);
    connect(widget, &GLWidget::OnTimeStepChange, time_step_input,
            &QDoubleSpinBox::setValue);

    setWindowTitle(tr(kWindowTitle));
    setFocus();
}

void GLWindow::CreateInputFields() {
    mass_label = new QLabel("Mass");
    spring_constant_label = new QLabel("K");
    damping_constant_label = new QLabel("Damping");
    rest_length_label = new QLabel("Rest Length");
    time_step_label = new QLabel("Time Step");

    mass_input->setRange(Mass::kMinimumMassValue, Mass::kMaximumMassValue);
    mass_input->setSingleStep(10);

    spring_constant_input->setRange(Spring::kMinimumSpringConstantValue,
                                    Spring::kMaximumSpringConstantValue);
    spring_constant_input->setSingleStep(10);

    damping_constant_input->setRange(Spring::kMinimumDampingValue,
                                     Spring::kMaximumDampingValue);
    damping_constant_input->setSingleStep(
        toolbox::numerics::LinearInterpolation(Spring::kMinimumDampingValue,
                                               Spring::kMaximumDampingValue,
                                               1.f / 100));

    rest_length_input->setRange(Spring::kMinimumSpringRestLengthValue,
                                Spring::kMaximumSpringRestLengthValue);
    rest_length_input->setSingleStep(1);

    time_step_input->setDecimals(6);
    time_step_input->setRange(MassSpringSystem::kMinimumTimeStepChangeValue,
                              MassSpringSystem::kMaximumTimeStepChangeValue);
    time_step_input->setSingleStep(toolbox::numerics::LinearInterpolation(
        MassSpringSystem::kMinimumTimeStepChangeValue,
        MassSpringSystem::kMaximumTimeStepChangeValue, 1.f / 100));
}

void GLWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    } else {
        QWidget::keyPressEvent(event);
    }
}
