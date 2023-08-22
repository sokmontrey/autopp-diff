#include <nodeflow/node.h>
#include <nodeflow/operator.h>

#include <Eigen/Dense>

using namespace nodeflow;

int main() {
    Node a = Node::Vector(1);

    a.print();

    return 0;
}

//TODO: finished apply constant
