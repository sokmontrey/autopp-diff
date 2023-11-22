#include <nodeflow/nodeflow.h>

using namespace nodeflow;

int main() {

    Node a = Node::Scalar(3.14);
    Node b = Node::Scalar(0.25);

    Mul m(&a, &b);
    Sin f(&m);
    f.finished();

    f.forward();
    f.print();

    return 0;
}
