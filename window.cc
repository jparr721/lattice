#include "window.h"
#include "gl_widget.h"
#include "main_window.h"

#include <iostream>

#include <QGuiApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QSlider>
#include <QVBoxLayout>

Window::Window(MainWindow* _main_window) : main_window(_main_window) {
    widget = new GLWidget;

    QLabel* mass_label = new QLabel("Mass");
    mass_slider = CreateSlider();

    QLabel* spring_constant_label = new QLabel("K");
    spring_constant_slider = CreateSlider();

    QLabel* damping_constant_label = new QLabel("Damping");
    damping_constant_slider = CreateSlider();

    QLabel* rest_length_label = new QLabel("Rest Length");
    rest_length_slider = CreateSlider();

    QLabel* time_step_label = new QLabel("Time Step");
    time_step_slider = CreateSlider();

    end_sim_button = new QPushButton(tr("End"), this);

    connect(mass_slider, &QSlider::valueChanged, widget, &GLWidget::SetMass);
    connect(spring_constant_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetSpringConstant);
    connect(damping_constant_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetSpringDampingConstant);
    connect(rest_length_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetSpringRestLength);
    connect(time_step_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetTimeStep);
    connect(end_sim_button, &QPushButton::clicked, this,
            &Window::EndSimulation);

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addWidget(widget);
    main_layout->addWidget(end_sim_button, 0, Qt::AlignTop);

    /** Mass Label and Slider **/
    main_layout->addWidget(mass_label, 0, Qt::AlignTop);
    main_layout->addWidget(mass_slider, 0, Qt::AlignTop);

    /** Spring Label and Slider **/
    main_layout->addWidget(spring_constant_label, 0, Qt::AlignTop);
    main_layout->addWidget(spring_constant_slider, 0, Qt::AlignTop);

    /** Damping Label and Slider **/
    main_layout->addWidget(damping_constant_label, 0, Qt::AlignTop);
    main_layout->addWidget(damping_constant_slider, 0, Qt::AlignTop);

    /** Rest Length Label and Slider **/
    main_layout->addWidget(rest_length_label, 0, Qt::AlignTop);
    main_layout->addWidget(rest_length_slider, 0, Qt::AlignTop);

    /** Time Step Label and Slider **/
    main_layout->addWidget(time_step_label, 0, Qt::AlignTop);
    main_layout->addWidget(time_step_slider, 0, Qt::AlignTop);

    setLayout(main_layout);

    mass_slider->setValue(GLWidget::kMinimumMassSliderValue);
    spring_constant_slider->setValue(
        GLWidget::kMinimumSpringConstantSliderValue);
    damping_constant_slider->setValue(GLWidget::kMinimumDampingSliderValue);
    rest_length_slider->setValue(GLWidget::kMinimumSpringRestLengthSliderValue);
    time_step_slider->setValue(GLWidget::kMinimumTimeStepChangeSliderValue);

    setWindowTitle(tr(kWindowTitle));
}

QSlider* Window::CreateSlider() {
    auto slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setSingleStep(5);
    slider->setPageStep(5);
    slider->setTickInterval(5);
    slider->setTickPosition(QSlider::TicksRight);

    return slider;
}

void Window::EndSimulation() {
    if (parent()) {
        setParent(nullptr);
        setAttribute(Qt::WA_DeleteOnClose);
        move(QGuiApplication::primaryScreen()->size().width() / 2 - width() / 2,
             QGuiApplication::primaryScreen()->size().height() / 2 -
                 height() / 2);
        end_sim_button->setText(tr("Start Simulation"));
        show();
    } else {
        if (!main_window->centralWidget()) {
            if (main_window->isVisible()) {
                setAttribute(Qt::WA_DeleteOnClose, false);
                end_sim_button->setText(tr("End Simulation"));
                main_window->setCentralWidget(this);
            } else {
                QMessageBox::information(nullptr, tr("Cannot Start"),
                                         tr("Main Window Already Closed"));
            }
        } else {
            QMessageBox::information(nullptr, tr("Cannot Start"),
                                     tr("Simulation Already Running"));
        }
    }
}

void Window::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    } else {
        QWidget::keyPressEvent(event);
    }
}