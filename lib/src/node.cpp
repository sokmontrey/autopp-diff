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

Eigen::MatrixXd& Node::getValue(){
    return this->value;
}

Eigen::MatrixXd& Node::forward(){
    return this->value;
}
