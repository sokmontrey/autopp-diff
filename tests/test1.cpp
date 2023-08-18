#include <nodeflow/node.h>
#include <Eigen/Dense>

using namespace Eigen;
using namespace nodeflow;

int main() {
  Node a = VectorXd::Random(2);
  Node b = VectorXd::Random(2);

  OperatorNode<2> f{&a, &b};
  OperatorNode<1> f2{&f};

  f2.forward();

  return 0;
}
