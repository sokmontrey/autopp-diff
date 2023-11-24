#include <nodeflow/nodeflow.h>

using namespace nodeflow;

int main() {
    // f = sin(a + b)
    Graph f ("sin(add(a, b))", {
        //make node "a" and "b" a scalar (a number, 1x1 matrix)
        //directly in the constructor
        {"a", Node::Scalar(0.1)},
        {"b", Node::Scalar(0.25)}
    });

    //calculate sin(0.1 + 0.25)
    f.forward().print();

    //change variable Node
    f
        .setNode("a", Node::Scalar(0.2))
        .setNode("b", Node::Random()) //random scalar value 0-1
        .forward() //calculate sin(0.2 + 0.1)
        .print()
    ;

    return 0;
}
