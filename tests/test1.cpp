#include <nodeflow/node.h>
#include <nodeflow/operator.h>
#include <Eigen/Dense>

using namespace Eigen;
using namespace nodeflow;

int main() {
    Node a = VectorXd::Random(5);
    a.constant();

    ReLU f {&a};

    f.finished();
    f.backward();

    return 0;
}

//TODO: finished apply constant
