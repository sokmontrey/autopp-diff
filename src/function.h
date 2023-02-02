#ifndef OPERATOR_H
#define OPERATOR_H

#include <cmath>

//tns = tensor
namespace tns{

//TODO: match sizes
template <typename TENSOR_TYPE>
struct Add{
	static void evaluateTo(TENSOR_TYPE *to_be_assign, TENSOR_TYPE *a, TENSOR_TYPE *b=nullptr);
	static TENSOR_TYPE evaluate(TENSOR_TYPE &a, TENSOR_TYPE &b);
};

template <typename TENSOR_TYPE>
struct ReLU{
	static void evaluateTo(TENSOR_TYPE *to_be_assign, TENSOR_TYPE *a, TENSOR_TYPE *b=nullptr);
	static TENSOR_TYPE evaluate(TENSOR_TYPE &a);
};

}

#endif //OPERATOR_H
