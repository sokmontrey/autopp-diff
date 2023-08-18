#include <nodeflow/node.h>
#include <Eigen/Dense>

using namespace Eigen;
using namespace nodeflow;

int main() {
  Node<VectorXd> b = VectorXd(3);

  b() << 1, 2, 3;
  b.print();

  return 0;
}
