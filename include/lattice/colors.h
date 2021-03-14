#pragma once

#include <string>

#include <Eigen/Dense>

namespace Colors {
float ScaleRGB(float intensity);

inline const Eigen::Vector3f kRed = Eigen::Vector3f(ScaleRGB(255.f), 0.f, 0.f);
inline const Eigen::Vector3f kGreen = Eigen::Vector3f(0.f, ScaleRGB(255.f), 0.f);
inline const Eigen::Vector3f kBlue = Eigen::Vector3f(0.f, 0.f, ScaleRGB(255.f));
inline const Eigen::Vector3f kPurple =
    Eigen::Vector3f(ScaleRGB(153.f), ScaleRGB(51.f), ScaleRGB(255.f));
inline const Eigen::Vector3f kPink =
    Eigen::Vector3f(ScaleRGB(255.f), ScaleRGB(51.f), ScaleRGB(255.f));
} // namespace Colors
