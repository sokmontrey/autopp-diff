#include <nodeflow/nodeflow.h>

using namespace nodeflow;

int main() {
    Graph f ("add(a, b)", {
         {"a", Node::Scalar(1)},
         {"b", Node::Scalar(1)}
    });

    Graph g ("add($f, b)", {
        {"b", Node::Scalar(5)}
    }, {
        {"$f", f}
    });

    g.forward().print();

    return 0;
}
