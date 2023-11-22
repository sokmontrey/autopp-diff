#include <nodeflow/nodeflow.h>

using namespace nodeflow;

int main() {
    Graph h1("relu(add(mul(w,x),b))",{
        {"x",Node::Scalar(1)},
        {"w",Node::Random(20,1)},
        {"b",Node::Random(20)}
    });

    Graph h2("relu(add(mul(w,$x),b))",{
        {"w",Node::Random(20,20)},
        {"b",Node::Random(20)}
    },{
        {"$x", h1.getF()}
    });

    Graph output("tanh(add(mul(w,$x),b))",{
        {"w",Node::Random(1,20)},
        {"b",Node::Random(1)}
    },{
        {"$x", h2.getF()}
    });

    Graph error("sum(pow(sub($output,t),2))",{
        {"t", Node::Random(1)}
    },{
        {"$output", output.getF()}
    });

    return 0;
}
