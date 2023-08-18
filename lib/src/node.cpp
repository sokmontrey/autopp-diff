#include <nodeflow/node.h>
#include <iostream>
#include <Eigen/Dense>

using namespace nodeflow;

template <typename DTYPE>
Node<DTYPE>::Node(DTYPE initial_data){
    _data = initial_data;
}

template <typename DTYPE>
void Node<DTYPE>::print(){
    std::cout << "Node: " << typeid(DTYPE).name() << "\n"
        << "----\n"
        << _data << "\n"
        << "----\n";
}

template <typename DTYPE>
DTYPE& Node<DTYPE>::operator()(){
    return _data;
}

template class Node<int>;
template class Node<double>;
template class Node<Eigen::VectorXd>;
template class Node<Eigen::MatrixXd>;
template class Node<Eigen::VectorXi>;
template class Node<Eigen::MatrixXi>;
