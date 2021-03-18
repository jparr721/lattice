#pragma once

#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    constexpr static int kWidth = 1200;
    constexpr static int kHeight = 800;

    MainWindow();
    ~MainWindow() override = default;
};
