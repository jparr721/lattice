#pragma once

#include <QWidget>
#include <QTabWidget>

class TabWidget : public QWidget {
    Q_OBJECT

  public:
    explicit TabWidget(QWidget* parent = nullptr);

  private:
    QTabWidget* tab_widget;
};
