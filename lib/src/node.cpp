#include <nodeflow/node.h>

using namespace nodeflow;

Node::Node(Eigen::MatrixXd initial_value){
    this->value = initial_value;
}

void Node::print(){
    std::cout << "Node: " << " (Matrix) " << "\n"
        << "----\n"
        << this->value << "\n"
        << "----\n";
}

Eigen::MatrixXd& Node::operator()(){
    return this->value;
}

template <unsigned int NINPUT>
OperatorNode<NINPUT>::OperatorNode(
    std::initializer_list<Node*> input_list
){
    for(size_t i=0; i<NINPUT; i++){
        this->inputs[i] = *(input_list.begin() + i);
    }
}

template class OperatorNode<1>;
template class OperatorNode<2>;
