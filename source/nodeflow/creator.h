#pragma once

#include <nodeflow/nodeflow.h>

namespace nodeflow {

    Node Number(double value){
        return Node::Scalar(value);
    }
    Node Scalar(double value){
        return Node::Scalar(value);
    }
    Node Vector(std::initializer_list<double> value){
        return Node::Vector(value);
    }
    Node Matrix(std::initializer_list<std::initializer_list<double>> value){
        return Node::Matrix(value);
    }

} //namespace nodeflow