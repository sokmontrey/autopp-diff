#pragma once

#include <nodeflow/node.h>

namespace nodeflow{
namespace op{
namespace basic{

//TODO:
// Pow x
// Square Root
// Invert
// Subtract
// Resieprocal
//
// Sin, Cos, Tan
//
// Exp, Ln, Log
//
// Max, Min
//
// PiecesWise

class Add: public OperatorNode<2>{
    using OperatorNode<2>::OperatorNode;

    void compute() override {
        this->value =
            this->getInput(0)
            +
            this->getInput(1)
        ;
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->outer_derivative;
    }
};

class Mul: public OperatorNode<2>{
    using OperatorNode<2>::OperatorNode;

    void compute() override {
        this->value =
            this->getInput(0)
            *
            this->getInput(1)
        ;
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override{
        if(input_wrt_index){ //second input
            return 
                this->getInput(0).transpose() 
                * 
                this->outer_derivative;
        }else{ // first input
            return 
                this->outer_derivative
                *
                this->getInput(1).transpose();
        }
    }
};

class Pow: public OperatorNode<1>{
    private:
        double exponent;
    public:
        Pow(std::initializer_list<Node*> input_list, double exponent)
        :OperatorNode<1>(input_list), exponent(exponent) { }

    void compute() override{
        this->value = this->getInput().array().pow(this->exponent);
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->getInput()
            .array()
            .pow(this->exponent - 1) 
            * this->exponent
            * this->outer_derivative.array()
        ;
    }
};

class Sqrt:public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override{
        this->value = this->getInput().array().sqrt();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return 
            (
                1 / (2 * this->getInput().array().sqrt())
            ) * this->outer_derivative.array() 
        ;
    }
};

}//namespace basic

namespace nn{

class ReLU: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override{
        this->value = 
            this->getInput().cwiseMax(0)
        ;
    }

    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return (this->value.array() > 0).cast<double>();
    }
};

}//namespace nn
}//namespace op
}//namespace nodeflow
