#include "tab_widget.h"
#include "plots_widget.h"
#include "gl_window.h"

#include <QVBoxLayout>

TabWidget::TabWidget(QWidget* parent) : QWidget(parent) {
    tab_widget = new QTabWidget;
    const auto gl_window = new Window;
    tab_widget->addTab(gl_window, tr("Simulation"));
    tab_widget->addTab(new PlotsWidget(gl_window), tr("Plots"));

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addWidget(tab_widget);
    setLayout(main_layout);
}
