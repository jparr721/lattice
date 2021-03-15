#pragma once

#include <string>

#include <Eigen/Dense>

class Colors {
  public:
        static float ScaleRGB(float intensity) { return intensity / 255.0f; }

        inline static const Eigen::Vector3f kRed = Eigen::Vector3f(ScaleRGB(255.f), 0.f, 0.f);
        inline static const Eigen::Vector3f kGreen = Eigen::Vector3f(0.f, ScaleRGB(255.f), 0.f);
};

