#include "main_window.h"
#include "window.h"

#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

MainWindow::MainWindow() { OnAddNew(); }

void MainWindow::OnAddNew() { setCentralWidget(new Window(this)); }
