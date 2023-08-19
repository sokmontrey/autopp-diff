#include <nodeflow/node.h>
#include <Eigen/Dense>

using namespace Eigen;
using namespace nodeflow;

int main() {
    Node a = VectorXd::Random(10);
    Node b = VectorXd::Random(2);

    ReLU f {&a};

    f.forward();

    return 0;
}
