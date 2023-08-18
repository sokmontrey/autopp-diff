#pragma once

#include <iostream>
#include <Eigen/Dense>
#include <variant>

namespace nodeflow{

class Node{
    protected:
        bool is_differentiatable = true;
        Eigen::MatrixXd value;

    public:
        Node() = default;
        Node(Eigen::MatrixXd initial_value);

        virtual void print();
        virtual Eigen::MatrixXd& operator()();

        virtual void forward(){
            std::cout << "Hello from Node" << std::endl;
        }
};

template <unsigned int NINPUT>
class OperatorNode: public Node{
    protected:
        Eigen::MatrixXd value;

        Node* inputs[NINPUT];

    public:
        OperatorNode() = default;
        OperatorNode(std::initializer_list<Node*> input_list);

        void forward() override{

            std::cout << "Hello from Op" << std::endl;

            for(int i=0; i<NINPUT; i++){
                this->inputs[i]->forward();
            }
        }
};

}//namespace nodeflow

