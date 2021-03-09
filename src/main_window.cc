#include <lattice/gl_window.h>
#include <lattice/main_window.h>
#include <lattice/shape_spec.h>
#include <lattice/gl_window.h>

#include <memory>

#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

MainWindow::MainWindow() {
    auto shape_spec = std::make_shared<ShapeSpec>();

    setFixedSize(kWidth, kHeight);
    setCentralWidget(new GLWindow(shape_spec));
}
