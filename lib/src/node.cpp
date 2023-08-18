#include <nodeflow/node.h>

using namespace nodeflow;

template <typename DTYPE>
Node<DTYPE>::Node(DTYPE initial_data){
    _data = initial_data;
}

template class Node<int>;
template class Node<double>;
