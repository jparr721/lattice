#pragma once

#include <lattice/generator.h>

#include <QPushButton>
#include <QWidget>

class ConfigurationWidget : public QWidget {
    Q_OBJECT

  public:
    ConfigurationWidget(QWidget* parent = nullptr);

  private slots:
    void SaveSpecification(generator::MSSConfig mss);

  private:
    // Widget Combo Boxes

    // Widget Action Buttons
    QPushButton* save_button;
    QPushButton* cancel_button;
};