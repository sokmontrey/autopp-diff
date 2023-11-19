#include <nodeflow/nodeflow.h>
#include <Eigen/Dense>

using namespace nodeflow;
/**
    Node a = Node::Matrix(2, 3, 1);
    Node b = Node::Matrix(3, 2, 1);

    Mul f(&a, &b);

    f.finished();
    f.forward();
    f.print();

    f.backward();
    std::cout << a.getGrad() << std::endl;
*/

int main() {
    Graph f("add(a, PI)", {
        {"a", Number(3.14)},
    });

    // f.setNode("a", Node::Scalar(3.14)).setNode("b", Node::Scalar(0.25));

    f.finished().forward().print();

    return 0;
}
