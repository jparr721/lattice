#pragma once

#include <string>

#include <QVector3D>

namespace colors {
float ScaleRGB(float intensity);

const QVector3D kRed = QVector3D(ScaleRGB(255.f), 0.f, 0.f);
const QVector3D kGreen = QVector3D(0.f, ScaleRGB(255.f), 0.f);
const QVector3D kBlue = QVector3D(0.f, 0.f, ScaleRGB(255.f));
const QVector3D kPurple =
    QVector3D(ScaleRGB(153.f), ScaleRGB(51.f), ScaleRGB(255.f));
const QVector3D kPink =
    QVector3D(ScaleRGB(255.f), ScaleRGB(51.f), ScaleRGB(255.f));

QVector3D from_string(const std::string& color);
} // namespace colors
