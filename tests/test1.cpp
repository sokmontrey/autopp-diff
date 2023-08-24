#include <nodeflow/node.h>
#include <nodeflow/operator.h>

#include <Eigen/Dense>

using namespace nodeflow;
using namespace op::basic;

/*
 * To finalize the graph created
 *      call .foward() then .finished() on the last node
 */

int main() {
    Node a = Node::Matrix({{0.5, -3}, {-1 , 0}});

    nodeflow::op::trig::Tan f{&a};

    f.forward();
    f.finished();
    f.print();

    f.backward();
    std::cout << a.getGrad() << std::endl;

    return 0;
}

//TODO:
// Pow ---------
// Square Root ----------
// Invert ----------
// Subtract -------
// Resieprocal -----
//
// Sin, Cos, Tan
//
// Exp, Ln, Log
//
// Max, Min
//
// PiecesWise
//
// Sigmoid
// Tanh
// Softmax
