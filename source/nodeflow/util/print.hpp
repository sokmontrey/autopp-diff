#pragma once

#include <Eigen/Dense>
#include <iostream>

namespace nodeflow {

inline void print(const Eigen::MatrixXd &mat) { std::cout << mat << std::endl; }

} // namespace nodeflow
