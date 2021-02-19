#include "colors.h"

namespace colors {
QVector3D from_string(const std::string& color) {
    if (color == "RED") {
        return kRed;
    } else if (color == "GREEN") {
        return kGreen;
    } else {
        return kBlue;
    }
}
} // namespace colors
