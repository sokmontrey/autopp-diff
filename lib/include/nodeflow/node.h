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

        int num_parent = 0;
        int parent_called_count = 0;

    public:
        Node() = default;
        Node(Eigen::MatrixXd initial_value){
            this->value = initial_value;
        }
        void operator=(Eigen::MatrixXd new_value){
            this->value = new_value;
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
        Eigen::MatrixXd& getGrad(){
            return this->outer_derivative;
        }

        void finished(){
            this->num_parent++;
            this->rows = this->value.rows();
            this->cols = this->value.cols();
        }
        void reset(){
            this->is_value_ready = false;
            this->parent_called_count = 0;
        }

        virtual Eigen::MatrixXd& forward(){
            return this->value;
        }
        virtual void backward(Eigen::MatrixXd partial_outer_derivative){
            if(!this->parent_called_count) {
                this->outer_derivative = partial_outer_derivative;
            }else{
                this->outer_derivative += partial_outer_derivative;
            }

            this->parent_called_count++;
        }
};

template <unsigned int NINPUT>
class OperatorNode: public Node{
    protected:
        Node* inputs[NINPUT];

        virtual void compute() = 0;
        virtual Eigen::MatrixXd derivative(size_t input_wrt_index) = 0;
        // virtual void derivative(size_t input_index, MatrixXd chain_value) = 0;
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
            this->parent_called_count = 0;

            for(size_t i=0; i<NINPUT; i++){
                this->inputs[i]->reset();
            }
        }

        void finished(){
            this->num_parent++;

            for(size_t i=0; i<NINPUT; i++){
                this->inputs[i]->finished();
            }

            this->compute();
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
        void backward(Eigen::MatrixXd partial_outer_derivative) override {
            if(!this->parent_called_count) {
                this->outer_derivative = partial_outer_derivative;
            }else{
                this->outer_derivative += partial_outer_derivative;
            }

            this->parent_called_count++;

            if(this->parent_called_count >= this->num_parent){
                for(size_t i=0; i<NINPUT; i++){
                    Eigen::MatrixXd partial_derivative = this->derivative(i);
                    this->inputs[i]->backward(partial_derivative);
                }
            }
        }
};


}//namespace nodeflow

