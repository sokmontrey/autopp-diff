#include <nodeflow/nodeflow.h>

using namespace nodeflow;

int main() {

    Graph f("#add(mul(a, sin(b)), c)", {
        {"a", Node::Scalar(3.14)},
        {"b", Node::Scalar(0.25)},
        {"c", Node::Scalar(0.25)}
    });

    f.forward().print();

    //test cases to check if the graph is being constanted correctly
    std::cout << "Testing constanting" << std::endl;
    for (int i=0; i<f.getOperatorLength(); i++) {
        std::cout << f.getOperatorNode(i)->isConstant() << std::endl;
    }

    // Node a = Node::Scalar(3.14);
    // Node b = Node::Scalar(0.25);
    //
    // Mul m(&a, &b);
    // Sin f(&m);
    //
    // m.constant();
    //
    // f.finished();
    // f.forward();
    // f.backward();

    return 0;
}
