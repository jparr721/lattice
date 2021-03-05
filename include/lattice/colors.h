#pragma once

#include <string>

#include <Eigen/Dense>

namespace colors {
float ScaleRGB(float intensity);

const Eigen::Vector3f kRed = Eigen::Vector3f(ScaleRGB(255.f), 0.f, 0.f);
const Eigen::Vector3f kGreen = Eigen::Vector3f(0.f, ScaleRGB(255.f), 0.f);
const Eigen::Vector3f kBlue = Eigen::Vector3f(0.f, 0.f, ScaleRGB(255.f));
const Eigen::Vector3f kPurple =
    Eigen::Vector3f(ScaleRGB(153.f), ScaleRGB(51.f), ScaleRGB(255.f));
const Eigen::Vector3f kPink =
    Eigen::Vector3f(ScaleRGB(255.f), ScaleRGB(51.f), ScaleRGB(255.f));

Eigen::Vector3f from_string(const std::string& color);
} // namespace colors
