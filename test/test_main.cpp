#include <nodeflow/nodeflow.h>
#include <Eigen/Dense>

using namespace nodeflow;

/*
 * To finalize the graph created
 *      call .foward() then .finished() on the last node
 */

//TODO: manually set rows and cols
int main() {
    Node a = Node::Random(4, 1);
    Node b = Node::Random(1, 1);

    Mul f(&a, &b);

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
