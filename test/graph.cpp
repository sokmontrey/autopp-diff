#include <nodeflow/nodeflow.h>

using namespace nodeflow;

int main() {
    Graph f ("sin(add(a, b))");

    f.setNode("a", Node::Scalar(0.1));
    f.setNode("b", Node::Scalar(0.25));
    f.finished();

    //calculate sin(0.1 + 0.25)
    f.forward().print();

    f.backward();
    std::cout << "da: " << f.getGrad("a") << std::endl;
    std::cout << "db: " << f.getGrad("b") << std::endl;

    //change variable Node
    f
        .setNode("a", Node::Vector({1,2,3}))
        .setNode("b", Node::Vector({1,2,3})) 
        .forward()
        .print()
    ;
    f.finished();
    f.backward();
    std::cout << "da: " << f.getGrad("a") << std::endl;
    std::cout << "db: " << f.getGrad("b") << std::endl;
    //change variable Node
    f
        .setNode("a", Node::Random(2, 3))
        .setNode("b", Node::Random(2, 3))
        .forward()
        .print()
    ;
    f.finished();
    f.backward();
    std::cout << "da: " << f.getGrad("a") << std::endl;
    std::cout << "db: " << f.getGrad("b") << std::endl;

    return 0;
}
