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

        void print();
        Eigen::MatrixXd& operator()();
        Eigen::MatrixXd& getValue();

        virtual Eigen::MatrixXd& forward();
};

template <unsigned int NINPUT>
class OperatorNode: public Node{
    protected:
        Node* inputs[NINPUT];

        virtual void compute(){
            this->value;
        }

    public:
        OperatorNode() = default;
        OperatorNode(std::initializer_list<Node*> input_list){
            for(size_t i=0; i<NINPUT; i++){
                auto input = *(input_list.begin() + i);
                this->inputs[i] = input;
            }
        }

        Eigen::MatrixXd& forward() override{
            for(int i=0; i<NINPUT; i++){
                this->inputs[i]->forward();
            }
            this->compute();
            return this->value;
        }
};

class ReLU: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;
    protected:
        void compute() override {
            this->value = this->inputs[0]->getValue().cwiseMax(0);
        }
};

}//namespace nodeflow

