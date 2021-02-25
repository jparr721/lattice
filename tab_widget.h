#pragma once

#include <QTabWidget>
#include <QWidget>

class TabWidget : public QWidget {
    Q_OBJECT

  public:
    explicit TabWidget(QWidget* parent = nullptr);

  private:
    QTabWidget* tab_widget;
};
