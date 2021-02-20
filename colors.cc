#include "colors.h"

#include <cmath>

namespace colors {
float ScaleRGB(float intensity) { return intensity / 255.0f; }

QVector3D from_string(const std::string& color) {
    if (color == "RED") {
        return kRed;
    } else if (color == "GREEN") {
        return kGreen;
    } else if (color == "PURPLE") {
        return kPurple;
    } else if (color == "PINK") {
        return kPink;
    } else {
        return kBlue;
    }
}
} // namespace colors
