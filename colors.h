#pragma once

#include <string>

#include <QVector3D>

namespace colors {
const QVector3D kRed = QVector3D(1.f, 0.f, 0.f);
const QVector3D kGreen = QVector3D(0.f, 1.f, 0.f);
const QVector3D kBlue = QVector3D(0.f, 0.f, 1.f);

QVector3D from_string(const std::string& color);
} // namespace colors
