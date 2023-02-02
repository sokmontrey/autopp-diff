#ifndef OPERATOR_H
#define OPERATOR_H

#include <cmath>

//tns = tensor
namespace tns{

//TODO: match sizes
template <typename TENSOR_TYPE>
struct Add{
	static TENSOR_TYPE evaluate(TENSOR_TYPE &a, TENSOR_TYPE &b){
		TENSOR_TYPE result;
		for(size_t i=0; i<result.getTotalSize(); i++){
			result.setValue(i, a.getValue(i) + b.getValue(i));
		}
		return result;
	}
	static void evaluateTo(
			TENSOR_TYPE *to_be_assign, 
			TENSOR_TYPE *a, 
			TENSOR_TYPE *b=nullptr){

		for(size_t i=0; i<to_be_assign->getTotalSize(); i++){
			to_be_assign->setValue(i, a->getValue(i) + b->getValue(i));
		}
	}
};

template <typename TENSOR_TYPE>
struct ReLU{
	static TENSOR_TYPE evaluate(TENSOR_TYPE &a){
		TENSOR_TYPE result;
		for(size_t i=0; i<result.getTotalSize(); i++){
			result.setValue(i, a(i) > 0 ? a(i) : 0);
		}
		return result;
	}
	static void evaluateTo(
			TENSOR_TYPE *to_be_assign, 
			TENSOR_TYPE *a,
			TENSOR_TYPE *b=nullptr){

		for(size_t i=0; i<to_be_assign->getTotalSize(); i++){
			to_be_assign->setValue(i, a->getValue(i) > 0 ? a->getValue(i) : 0);
		}
	}
};

}

#endif //OPERATOR_H
