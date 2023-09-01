#include <nodeflow/nodeflow.h>
#include <Eigen/Dense>

using namespace nodeflow;

/*
 * To finalize the graph created
 *      call .foward() then .finished() on the last node
 */

int main() {
    Node a = Node::Vector({1, 2, 3});
    Node b = Node::Vector({4, 5, 6});

    Sum f{&a};

    f.forward();
    f.finished();
    f.print();

    f.backward();
    std::cout << a.getGrad() << std::endl;

    return 0;
}

//TODO: col Vector with initializer_list
//TODO:
// Pow ---------
// Square Root ----------
// Invert ----------
// Subtract -------
// Resieprocal -----
//
// Sin, Cos, Tan -------
//
// Exp, Ln, Log -------
//
// Max, Min
//
// PiecesWise
//
// Sigmoid------
// Tanh-------
// Softmax
