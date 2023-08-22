#pragma once

#include <nodeflow/node.h>

namespace nodeflow{
namespace op{
namespace basic{

class Add: public OperatorNode<2>{
    using OperatorNode<2>::OperatorNode;

    void compute() override {
        this->value =
            this->inputs[0]->getValue()
            +
            this->inputs[1]->getValue()
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
            this->inputs[0]->getValue()
            *
            this->inputs[1]->getValue()
        ;
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override{
        if(input_wrt_index){ //second input
            return 
                this->inputs[0]->getValue().transpose() 
                * 
                this->outer_derivative;
        }else{ // first input
            return 
                this->outer_derivative
                *
                this->inputs[1]->getValue().transpose();
        }
    }
};

}//namespace basic

namespace nn{

class ReLU: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override{
        this->value = 
            this->inputs[0]->getValue().cwiseMax(0)
        ;
    }

    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return (this->value.array() > 0).cast<double>();
    }
};

}//namespace nn
}//namespace op
}//namespace nodeflow
