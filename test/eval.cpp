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

//TODO: Graph constructor with initial node values
// 

int main() {

  Graph f("add(a,b)");

  f.setNode("a", Node::Scalar(1)).setNode("b", Node::Scalar(5));

  f.finished().forward().print();

  return 0;
}
