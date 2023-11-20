#include <nodeflow/nodeflow.h>
#include <Eigen/Dense>

using namespace nodeflow;

int main() {
    /*
    Node w = Node::Random(3, 2);
    Node x = Node::Vector(2, 1);
    Mul m(&w, &x);

    Graph f("tanh(add($m, b))", {
        {"b", Node::Random(3, 1)},
    },{
        {"$m", &m},
    });

    f.forward().print();
    f.backward();
    std::cout << w.getGrad() << std::endl;
    */

    /*
    Graph f("add(a, PI)", {
        {"a", Node::Scalar(3.14)},
    });
    f.setNode("a", Node::Scalar(3.14)).setNode("b", Node::Scalar(0.25));
    f.forward().print();
    std::cout << f.backward().getGrad("w") << std::endl;
    */

    return 0;
}
