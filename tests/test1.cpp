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
    Node a = Node::Matrix({{2, -3}, {-1 , 0}});

    Pow f_1({&a}, 2);
    Subtract f{&f_1, &a};

    f.forward();
    f.finished();
    f.print();

    f.backward();
    std::cout<< f_1.getGrad() << std::endl;

    return 0;
}

//TODO:
// Pow ---------
// Square Root ----------
// Invert ----------
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
//
// Sigmoid
// Tanh
// Softmax
