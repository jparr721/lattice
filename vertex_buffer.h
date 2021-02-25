#pragma once

#include "mass.h"
#include "spring.h"

#include <memory>
#include <vector>

#include <Eigen/Dense>

class VertexBuffer {
  public:
    VertexBuffer() = default;
    ~VertexBuffer() = default;

    void ComputeVertexPoints();
    void ComputeShapes();

  private:
    // Always using triangles
    constexpr static int kVerticesPerShape = 3;

    // The pixel-size of the shape when rendered.
    int shape_size = 1;

    // How many triangles?
    int n_shapes;

    // The scale factor of the shape, letting us dynamically size it.
    float shape_scale_factor = 1;

    std::vector<Eigen::Vector3f> vertices;
    std::vector<Eigen::Vector3f> colors;

    // The center point to normalize around
    Eigen::Vector3f center_point;
};
