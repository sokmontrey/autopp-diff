#include <nodeflow/node.h>
#include <nodeflow/operator.h>

#include <Eigen/Dense>

using namespace nodeflow;
using namespace op::basic;

//TODO: Node::Scalar(initial_number);
//TODO: Node::Vector({1, 2,3})
//TODO: smae thing for Matrix
int main() {
    Node a = Node::Vector(1);
    a() << 9;

    Sqrt f{&a};

    f.finished();
    f.forward();
    f.backward();

    f.print();

    std::cout<< a.getGrad() << std::endl;

    return 0;
}

//TODO: finished apply constant
