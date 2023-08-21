#include <nodeflow/node.h>
#include <nodeflow/operator.h>
#include <Eigen/Dense>

using namespace Eigen;
using namespace nodeflow;

int main() {
    Node a = VectorXd(1);
    Node b = VectorXd(1, 1);

    a() << 5;
    b() << 10;

    Mul f {&b, &a};

    f.finished();

    f.backward();

    std::cout << b.getGrad() << std::endl;

    return 0;
}
