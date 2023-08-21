#pragma once

#include <iostream>
#include <Eigen/Dense>
#include <variant>

namespace nodeflow{

class Node{
    protected:
        bool is_differentiatable = true;
        bool is_value_ready = false;

        Eigen::MatrixXd value;
        Eigen::MatrixXd outer_derivative;

        unsigned int rows = 1;
        unsigned int cols = 1;

        void zeroOuterDerivative(){
            this->outer_derivative = Eigen::MatrixXd::Constant(
                this->rows,
                this->cols,
                0.0
            );
        }

    public:
        Node() = default;
        Node(Eigen::MatrixXd initial_value){
            this->value = initial_value;
            this->rows = initial_value.rows();
            this->cols = initial_value.cols();
        }
        void operator=(Eigen::MatrixXd new_value){
            this->value = new_value;
            this->rows = new_value.rows();
            this->cols = new_value.cols();
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

        void reset(){
            this->is_value_ready = false;
        }

        virtual Eigen::MatrixXd& forward(){
            return this->value;
        }
        // virtual void backward(){
        //     std::cout << "backward node" <<std::endl;
        // }
};

template <unsigned int NINPUT>
class OperatorNode: public Node{
    protected:
        bool is_leaf_node = false;

        Node* inputs[NINPUT];

        virtual void compute() = 0;
        // virtual void derivative(size_t input_index, MatrixXd chain_value) = 0;
        // TODO: breath first search first, then reset partial_outer_derivative later
    public:
        OperatorNode() = default;
        OperatorNode(std::initializer_list<Node*> input_list){
            for(size_t i=0; i<NINPUT; i++){
                auto input = *(input_list.begin() + i);
                this->inputs[i] = input;
            }
        }

        void reset(){
            if(!this->is_value_ready) return;

            this->is_value_ready = false;
            for(size_t i=0; i<NINPUT; i++){
                this->inputs[i]->reset();
            }
        }

        void finished(){
            this->is_leaf_node = true;
            this->forward();
            this->rows = this->value.rows();
            this->cols = this->value.cols();
        }

        Eigen::MatrixXd& forward() override{
            if(this->is_value_ready) return this->value;

            for(size_t i=0; i<NINPUT; i++){
                this->inputs[i]->forward();
            }
            this->compute();

            this->is_value_ready = true;
            return this->value;
        }

        void backward(){
            this->backward(
                Eigen::MatrixXd::Constant(
                    this->rows,
                    this->cols,
                    1.0
                )
            );
        }
        void backward(
            Eigen::MatrixXd partial_outer_derivative, 
        ){
            //TODO: reset outer_derivative
            //TODO: local gradient
            this->outer_derivative += partial_outer_derivative;
        }

        //TODO: pass chain value to compute derivative() and calculate grad directly
        // void backward(Eigen::MatrixXd chain_value) override{
        //     std::cout << "backward op" <<std::endl;
        //     for(size_t i=0; i<NINPUT; i++){
        //         this->derivative(i);
        //     }
        //     for(size_t i=0; i<NINPUT; i++){
        //         this->inputs[i]->backward();
        //     }
        // }
        //TODO: getter for grad
};


}//namespace nodeflow

