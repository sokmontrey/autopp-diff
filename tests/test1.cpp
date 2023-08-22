#include <nodeflow/node.h>
#include <nodeflow/operator.h>

#include <Eigen/Dense>

using namespace nodeflow;
using namespace op::basic;

int main() {
    Node a = Node::Vector(1);
    a() << 4;

    Pow f({&a}, 0.5);

    f.finished();
    f.forward();
    f.backward();

    f.print();

    std::cout<< a.getGrad() << std::endl;

    return 0;
}

//TODO: finished apply constant
