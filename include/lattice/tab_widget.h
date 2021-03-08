#pragma once

#include <lattice/shape_spec.h>

#include <memory>

#include <QTabWidget>
#include <QWidget>

class TabWidget : public QWidget {
    Q_OBJECT

  public:
    TabWidget(std::shared_ptr<ShapeSpec> shape_spec, QWidget* parent = nullptr);

  private:
    QTabWidget* tab_widget;
};
