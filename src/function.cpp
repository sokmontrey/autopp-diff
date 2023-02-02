#include "./function.h"

using namespace tns;

/*-------------------------------Add-----------------------------*/
template <typename TENSOR_TYPE>
void Add<TENSOR_TYPE>::evaluateTo(
		TENSOR_TYPE *to_be_assign,
		TENSOR_TYPE *a,
		TENSOR_TYPE *b
){
	for(size_t i=0; i<to_be_assign->getTotalSize(); i++){
		to_be_assign->setValue(i, a->getValue(i) + b->getValue(i));
	}
}

template <typename TENSOR_TYPE>
TENSOR_TYPE Add<TENSOR_TYPE>::evaluate(
		TENSOR_TYPE &a,
		TENSOR_TYPE &b
){
	TENSOR_TYPE result;
	Add<TENSOR_TYPE>::evaluateTo(&result, &a, &b);
	return result;
}

/*-------------------------------ReLU-----------------------------*/
template <typename TENSOR_TYPE>
void ReLU<TENSOR_TYPE>::evaluateTo(
		TENSOR_TYPE *to_be_assign,
		TENSOR_TYPE *a,
		TENSOR_TYPE *b
){
	for(size_t i=0; i<to_be_assign->getTotalSize(); i++){
		to_be_assign->setValue(i, a->getValue(i) > 0 ? a->getValue(i) : 0);
	}
}

template <typename TENSOR_TYPE>
TENSOR_TYPE ReLU<TENSOR_TYPE>::evaluate(
		TENSOR_TYPE &a
){
	TENSOR_TYPE result;
	ReLU<TENSOR_TYPE>::evaluateTo(&result, &a);
	return result;
}
