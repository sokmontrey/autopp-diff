#ifndef OPERATOR_H
#define OPERATOR_H

#include <cmath>

//tns = tensor
namespace tns{

//TODO: match sizes
struct Add{
	template <typename TENSOR_TYPE>
	static TENSOR_TYPE evaluate(TENSOR_TYPE &a, TENSOR_TYPE &b){
		TENSOR_TYPE result;
		for(size_t i=0; i<result.getTotalSize(); i++){
			result.setValue(i, a(i) + b(i));
		}
		return result;
	}
	template <typename TENSOR_TYPE>
	static void evaluate(
			TENSOR_TYPE &to_be_assign, 
			TENSOR_TYPE &a, 
			TENSOR_TYPE &b){

		for(size_t i=0; i<to_be_assign.getTotalSize(); i++){
			to_be_assign.setValue(i, a.getValue(i) + b.getValue(i));
		}
	}
};

}

#endif //OPERATOR_H
