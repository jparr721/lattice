#include "main_window.h"
#include "window.h"

#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

MainWindow::MainWindow() {
    QMenuBar* menu_bar = new QMenuBar;
    QMenu* menu_window = menu_bar->addMenu(tr("&Window"));
    QAction* add_new_sim = new QAction(menu_window);

    add_new_sim->setText(tr("Add New Simulator"));
    menu_window->addAction(add_new_sim);
    connect(add_new_sim, &QAction::triggered, this, &MainWindow::OnAddNew);
    setMenuBar(menu_bar);

    OnAddNew();
}

void MainWindow::OnAddNew() {
    if (!centralWidget()) {
        setCentralWidget(new Window(this));
    } else {
        QMessageBox::information(
            nullptr, tr("Cannot add a new window"),
            tr("Sim running, wait for it to finish, or end it."));
    }
}