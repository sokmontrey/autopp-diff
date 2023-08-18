#include <nodeflow/node.h>
#include <iostream>
#include <Eigen/Dense>

using namespace nodeflow;

template <typename DTYPE>
Node<DTYPE>::Node(DTYPE initial_data){
    _data = initial_data;
}

template class Node<int>;
template class Node<double>;
template class Node<Eigen::VectorXd>;
template class Node<Eigen::MatrixXd>;
