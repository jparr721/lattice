#pragma once

#include "mass.h"
#include "spring.h"

#include <string>
#include <vector>

#include <Eigen/Dense>

bool LoadObjFile(const std::string& obj_file_path,
                 std::vector<Eigen::Vector3f>& vertices);
