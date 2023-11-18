#include <nodeflow/nodeflow.h>
#include <Eigen/Dense>

using namespace nodeflow;

/*
 * To finalize the graph created
 *      call .foward() then .finished() on the last node
 */

//TODO: manually set rows and cols
int main() {
    Node x = Eigen::MatrixXd({
        {1, 2, 3},
        {4, 5, 6},
    });

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
