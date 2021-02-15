#pragma once

#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow();
    ~MainWindow() = default;

  private slots:
    void OnAddNew();
};