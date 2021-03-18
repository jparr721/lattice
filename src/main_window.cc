#include <lattice/configuration.h>
#include <lattice/gl_window.h>
#include <lattice/main_window.h>

MainWindow::MainWindow() {
    MSSConfig config = MSSConfig("sim1", 4, 2, 4);

    setFixedSize(kWidth, kHeight);
    setCentralWidget(new GLWindow(config));
}
