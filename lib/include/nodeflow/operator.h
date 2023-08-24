#pragma once

#include <nodeflow/node.h>

/*
 * Below is a template for create a custom operator node
 */

/*
class FunctionName: public OperatorNode<[number of input]>{
    //default constructor (check Pow for custom constructor)
    using OperatorNode<[number of input]>::OperatorNode;

    void compute() override {
        //Input Node(s) can be access by this->getInput(INPUT_INDEX)
        //      or this->getInput() for a signal input op 
        //      (too lazy to use 0 index)
        //Result from the calculation MUST be assigned to this->value
    }

    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        //MUST: return a matrix as the result to the taking the partial derivative on the function with respect to this->inputs[input_wrt_index]
        //MUST: use this->outer_derivative according to chain rule
        return ;
    }
};
*/

namespace nodeflow{
namespace op{
namespace basic{

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

class Invert: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = - this->getInput();
    }

    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return -this->outer_derivative;
    }
};

class Subtract: public OperatorNode<2>{
    using OperatorNode<2>::OperatorNode;

    void compute() override {
        this->value = this->getInput(0) - this->getInput(1);
    }

    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        if(input_wrt_index){ // Second input
            return -this->outer_derivative;
        }else{ // First input
            return this->outer_derivative;
        }
    }
};

class Inverse: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().inverse();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return -this->getInput().array().pow(2).inverse();
    }
};

}//namespace basic -------------------------------------------

namespace trig{

class Sin: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().sin();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->getInput().array().cos();
    }
};

class Cos: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().cos();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return -this->getInput().array().sin();
    }
};

class Tan: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().tan();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->getInput().array().cos().pow(2).inverse();
    }
};

}//namespace trig --------------------------------------------

namespace hyp{

class Sinh: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().sinh();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->getInput().array().cosh();
    }
};

class Cosh: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().cosh();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->getInput().array().sinh();
    }
};

class Tanh: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().tanh();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->getInput().array().cosh().pow(2).inverse();
    }
};

}//namespace hyp ---------------------------------------------

namespace exp{

class Exp: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().exp();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->getInput().array().exp();
    }
};

class Loge: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().log();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->getInput().array().inverse();
    }
};

}//namesapce exp-----------------------------------------------

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

}//namespace nn ----------------------------------------------
}//namespace op ----------------------------------------------
}//namespace nodeflow ----------------------------------------
