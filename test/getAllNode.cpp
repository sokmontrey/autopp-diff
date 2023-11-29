#include <nodeflow/nodeflow.h>

using namespace nodeflow;

int main() {
    Graph h1 ("relu(add(mul(w,#x),b))", {
        {"#x", Node::Random(1,1)},
        {"w", Node::Random(20,1)},
        {"b", Node::Random(20,1)}
    });

    Graph h2 ("relu(add(mul(w,$x),b))", {
        {"w", Node::Random(20,20)},
        {"b", Node::Random(20)},
    },{
        {"$x", &h1}
    });

    Graph h3 ("relu(add(mul(w,$x),b))", {
        {"w", Node::Random(20,20)},
        {"b", Node::Random(20)},
    },{
        {"$x", &h2}
    });

    Graph model ("tanh(add(mul(w,$x),b))", {
        {"w", Node::Random(1,20)},
        {"b", Node::Random()},
    },{
        {"$x", &h3}
    });

    std::vector<Node*> nodes = model.getAllLeaveNode(); //TODO

    for (auto& n : nodes) {
        std::cout << "something" << std::endl;
    }

    std::cout << nodes.size() << std::endl;

    h1.constant("w");

    std::vector<Node*> var = model.getAllVariableNode();

    for (auto& n : var) {
        std::cout << "var" << std::endl;
    }

    std::cout << var.size() << std::endl;

    return 0;
}
