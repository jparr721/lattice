#include <lattice/gl_window.h>
#include <lattice/plots_widget.h>
#include <lattice/tab_widget.h>

#include <QVBoxLayout>

TabWidget::TabWidget(std::shared_ptr<ShapeSpec> shape_spec, QWidget* parent)
    : QWidget(parent) {
    tab_widget = new QTabWidget;
    const auto gl_window = new GLWindow(shape_spec);
    tab_widget->addTab(gl_window, tr("Simulation"));
    tab_widget->addTab(new PlotsWidget(gl_window), tr("Plots"));

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addWidget(tab_widget);
    setLayout(main_layout);
}
