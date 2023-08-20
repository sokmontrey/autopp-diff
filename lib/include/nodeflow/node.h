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
        Node(Eigen::MatrixXd initial_value){
            this->value = initial_value;
        }

        void print(){
            std::cout << "Node: " << " (Matrix) " << "\n"
                << "----\n"
                << this->value << "\n"
                << "----\n";
        }

        Eigen::MatrixXd& operator()(){
            return this->value;
        }
        Eigen::MatrixXd& getValue(){
            return this->value;
        }
        virtual Eigen::MatrixXd& forward(){
            return this->value;
        }
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


}//namespace nodeflow

