#include "window.h"
#include "gl_widget.h"
#include "main_window.h"

#include <iostream>

#include <QChart>
#include <QChartView>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineSeries>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QSlider>
#include <QStringLiteral>
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

    // end_sim_button = new QPushButton(tr("End"), this);

    connect(mass_slider, &QSlider::valueChanged, widget, &GLWidget::SetMass);
    connect(spring_constant_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetSpringConstant);
    connect(damping_constant_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetSpringDampingConstant);
    connect(rest_length_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetSpringRestLength);
    connect(time_step_slider, &QSlider::valueChanged, widget,
            &GLWidget::SetTimeStep);
    // connect(end_sim_button, &QPushButton::clicked, this,
    //         &Window::EndSimulation);

    QVBoxLayout* main_layout = new QVBoxLayout;

    QHBoxLayout* data_layout = new QHBoxLayout;

    QVBoxLayout* parameters_layout = new QVBoxLayout;
    QLabel* parameters = new QLabel(tr("Parameters"));
    QtCharts::QLineSeries* series = new QtCharts::QLineSeries();
    series->append(0, 6);
    series->append(2, 4);
    QtCharts::QChart* chart = new QtCharts::QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Foo");
    QtCharts::QChartView* chart_view = new QtCharts::QChartView(chart);
    series->append(4, 6);
    series->append(6, 4);

    parameters_layout->addWidget(parameters);
    parameters_layout->addWidget(chart_view);

    data_layout->addWidget(widget);
    data_layout->addLayout(parameters_layout);

    data_layout->setStretchFactor(parameters_layout, 1);
    data_layout->setStretchFactor(widget, 2);

    main_layout->addLayout(data_layout);

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
    setFocus();
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

void Window::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    } else {
        QWidget::keyPressEvent(event);
    }
}