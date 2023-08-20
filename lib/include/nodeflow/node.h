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
        virtual void backward(){
            std::cout << "backward node" <<std::endl;
        }
};

template <unsigned int NINPUT>
class OperatorNode: public Node{
    protected:
        Node* inputs[NINPUT];
        //TODO: no partial, just grad 
        MatrixXd grad_dinputs[NINPUT];

        virtual void compute() = 0;
        virtual void derivative(size_t input_index, MatrixXd chain_value) = 0;

    public:
        OperatorNode() = default;
        OperatorNode(std::initializer_list<Node*> input_list){
            for(size_t i=0; i<NINPUT; i++){
                auto input = *(input_list.begin() + i);
                this->inputs[i] = input;
            }
        }

        Eigen::MatrixXd& forward() override{
            for(size_t i=0; i<NINPUT; i++){
                this->inputs[i]->forward();
            }
            this->compute();
            return this->value;
        }

        //TODO: pass chain value to compute derivative() and calculate grad directly
        void backward(MatrixXd chain_value) override{
            std::cout << "backward op" <<std::endl;
            for(size_t i=0; i<NINPUT; i++){
                this->derivative(i);
            }
            for(size_t i=0; i<NINPUT; i++){
                this->inputs[i]->backward();
            }
        }
        //TODO: getter for grad
};


}//namespace nodeflow

