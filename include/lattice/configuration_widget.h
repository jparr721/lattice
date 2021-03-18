#pragma once

#include <lattice/configuration.h>

#include <QPushButton>
#include <QWidget>

class ConfigurationWidget : public QWidget {
    Q_OBJECT

  public:
    explicit ConfigurationWidget(QWidget* parent = nullptr);

  private slots:
    void SaveSpecification(const MSSConfig& mss);

  private:
    // Widget Combo Boxes

    // Widget Action Buttons
    QPushButton* save_button;
    QPushButton* cancel_button;
};