#include "main_window.h"
#include "gl_window.h"
#include "tab_widget.h"

#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

MainWindow::MainWindow() {
    setFixedSize(kWidth, kHeight);
    setCentralWidget(new TabWidget(this));
}
