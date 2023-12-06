#include <nodeflow/nodeflow.h>
#include <matplot/matplot.h>
//
#define M_PI 3.14159265358979323846
using namespace nodeflow;
using namespace matplot;

int main() {
    Graph h1 ("tanh(add(mul(w,#x),b))", {
        {"#x", Node::Random(1,1)},
        {"w", Node::Random(20,1)},
        {"b", Node::Random(20,1)}
    });

    Graph h2 ("tanh(add(mul(w,$x),b))", {
        {"w", Node::Random(20,20)},
        {"b", Node::Random(20)},
    },{
        {"$x", &h1}
    });

    Graph h3 ("tanh(add(mul(w,$x),b))", {
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

    Graph error ("pow(sub(#t,$y),2)",{
        {"#t", Node::Scalar(0)},
    },{
        {"$y", &model}
    });

    std::vector<Node*> vars = model.getAllVariableNode();

    double learning_rate = 0.005;

    std::vector<double> x(100);
    std::vector<double> y(100);
    std::vector<double> p(100);

    for(int i=0; i<100; i++){
        x[i] = 2 * M_PI * i / 100;
        y[i] = sin(x[i]);
    }

    for(int i=0; i<100; i++){
        double loss = 0;

        for (int j=0; j<100; j++){
            h1.setValue("#x", x[j]);
            error.setValue("#t", y[j]);

            loss += error.forward().getValue();
            error.backward();

            for (auto var : vars){
                var->setMatrix(var->getMatrix() - learning_rate * var->getGrad());
            }
        }

        loss /= 100;
        std::cout << "loss: " << loss << std::endl;
    }

    for (int j=0; j<100; j++){
        h1.setValue("#x", x[j]);
        p[j] = model.forward().getValue();
    }

    plot(x,y);
    hold(on);
    plot(x,p);
    show();

    return 0;
}