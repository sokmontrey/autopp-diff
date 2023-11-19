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
    //a simple regression model
    Node w = Node::Random(3, 2);
    Node x = Node::Vector(2, 1);
    Mul m(&w, &x);

    Graph f("tanh(add($m, b))", {
        {"b", Node::Random(3, 1)},
    });

    f.forward().print();
    std::cout << f.backward().getGrad("w") << std::endl;

    // Graph f("add(a, PI)", {
    //     {"a", Node::Scalar(3.14)},
    // });

    // f.setNode("a", Node::Scalar(3.14)).setNode("b", Node::Scalar(0.25));

    // f.forward().print();
    //
    // std::cout << f.backward().getGrad("w") << std::endl;

    return 0;
}
