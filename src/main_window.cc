#include <lattice/colors.h>
#include <lattice/generator.h>
#include <lattice/gl_window.h>
#include <lattice/main_window.h>

#include <memory>

#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow() {
    generator::MSSConfig config = generator::Square(
        "sim1", 4 /* Width */, 2 /* Height */, 4 /* Depth */, colors::kRed);

    setFixedSize(kWidth, kHeight);
    setCentralWidget(new GLWindow(config));
}
