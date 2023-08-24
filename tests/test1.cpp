#include <nodeflow/node.h>
#include <nodeflow/operator.h>

#include <Eigen/Dense>

using namespace nodeflow;
using namespace op::basic;

int main() {
    Node a = Node::Matrix({{1}});

    Sqrt f{&a};

    f.finished();
    f.forward();
    f.backward();

    f.print();

    std::cout<< a.getGrad() << std::endl;

    return 0;
}

//TODO:
// Pow ---------
// Square Root ----------
// Invert
// Subtract
// Resieprocal
//
// Sin, Cos, Tan
//
// Exp, Ln, Log
//
// Max, Min
//
// PiecesWise
