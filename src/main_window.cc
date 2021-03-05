#include <lattice/main_window.h>
#include <lattice/gl_window.h>
#include <lattice/tab_widget.h>

#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

MainWindow::MainWindow() {
    setFixedSize(kWidth, kHeight);
    setCentralWidget(new TabWidget(this));
}
