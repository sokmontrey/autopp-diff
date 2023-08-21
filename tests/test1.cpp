#include <nodeflow/node.h>
#include <nodeflow/operator.h>
#include <Eigen/Dense>

using namespace Eigen;
using namespace nodeflow;

int main() {
    Node a = VectorXd::Random(2);
    Node b = MatrixXd::Random(3, 2);

    Mul f {&b, &a};

    f.finished();
    f.backward();

    return 0;
}
