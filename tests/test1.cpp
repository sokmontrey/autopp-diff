#include <nodeflow/node.h>
#include <Eigen/Dense>

int main() {
  nodeflow::Node<Eigen::MatrixXd> b = Eigen::MatrixXd::Random(2, 3);

  return 0;
}
