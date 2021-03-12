#pragma once

#include <lattice/generator.h>

#include <QPushButton>
#include <QWidget>

class ShapeSpecWidget : public QWidget {
    Q_OBJECT

  public:
    ShapeSpecWidget(QWidget* parent = nullptr);

  private slots:
    void SaveSpecification(generator::MSSConfig mss);

  private:
    // Widget Combo Boxes

    // Widget Action Buttons
    QPushButton* save_button;
    QPushButton* cancel_button;
};