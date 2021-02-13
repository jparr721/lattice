//
//  window.cpp
//  lattice
//
//  Created by Jarred Parr on 2/1/21.
//

#include "window.hpp"
#include "gl_widget.hpp"

#include <QtOpenGL/qgl.h>
#include <QtWidgets/qtwidgetsglobal.h>
#include <QtWidgets/qabstractslider.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qboxlayout.h>

void Window::CreateSlider(QLabel* label, QSlider* slider) {
    slider->setRange(0, 100);
    slider->setSingleStep(5);
    slider->setPageStep(5);
    slider->setTickInterval(5);
    slider->setTickPosition(QSlider::TicksRight);
}

Window::Window() {
    gl_widget = new GLWidget();

    // Set up sliders
    QLabel* mass_label = new QLabel("Mass");
    QSlider* mass_slider = new QSlider(Qt::Horizontal);
    CreateSlider(mass_label, mass_slider);
    
    QLabel* spring_label = new QLabel("Spring Constant");
    QSlider* spring_slider = new QSlider(Qt::Horizontal);
    CreateSlider(spring_label, spring_slider);
    
    QLabel* damping_label = new QLabel("Damping Constant");
    QSlider* damping_slider = new QSlider(Qt::Horizontal);
    CreateSlider(damping_label, damping_slider);
    
    QLabel* rest_length_label = new QLabel("Rest Length");
    QSlider* rest_length_slider = new QSlider(Qt::Horizontal);
    CreateSlider(rest_length_label, rest_length_slider);
    
    QLabel* time_step_label = new QLabel("Time Step Size");
    QSlider* time_step_slider = new QSlider(Qt::Horizontal);
    CreateSlider(time_step_label, time_step_slider);
    
    QPushButton* restart_button = new QPushButton("Restart");
    
    QVBoxLayout* window_layout = new QVBoxLayout();
    window_layout->addWidget(gl_widget);
    window_layout->addWidget();
}
