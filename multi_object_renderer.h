#pragma once

#include "mass.h"
#include "mass_spring_system.h"
#include "shape_spec.h"
#include "spring.h"

#include <memory>
#include <vector>

class MultiObjectRenderer {
  public:
    MultiObjectRenderer() = default;
    ~MultiObjectRenderer() = default;

    std::shared_ptr<MassSpringSystem> SimulationObject() {
        return mass_spring_system;
    }

    void Initialize();

  private:
    bool is_init = false;

    std::shared_ptr<MassSpringSystem> mass_spring_system;

    std::unique_ptr<ShapeSpec> digraph;
};
