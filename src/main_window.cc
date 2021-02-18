#include <lattice/main_window.h>
#include <lattice/window.h>

#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

MainWindow::MainWindow() {
    AddCentralWidget();
}

void MainWindow::AddCentralWidget() {
    setCentralWidget(new Window(this));
}
