#pragma once

#include <nodeflow/node.h>

namespace nodeflow{
    
class ReLU: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override{
        this->value = 
            this->inputs[0]->getValue().cwiseMax(0)
        ;
    }
};

class Add: public OperatorNode<2>{
    using OperatorNode<2>::OperatorNode;

    void compute() override {
        this->value =
            this->inputs[0]->getValue()
            +
            this->inputs[1]->getValue()
        ;
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
};

}//namespace nodeflow
