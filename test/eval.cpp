#include <nodeflow/nodeflow.h>

using namespace nodeflow;

int main() {
    Graph f("add(a, mul(2,PI))", {
        {"a", Node::Scalar(3.14)},
    });
    f.forward().print();

    return 0;
}
