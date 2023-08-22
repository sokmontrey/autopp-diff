#include <nodeflow/node.h>
#include <nodeflow/basic_operator.h>

#include <Eigen/Dense>

using namespace nodeflow;
using namespace op::basic;

int main() {
    Node a = Node::Vector(1);

    a.print();

    ReLU f {&a};

    return 0;
}

//TODO: finished apply constant
