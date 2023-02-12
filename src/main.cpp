#include <iostream>
#include "./node.cpp"

//TRIED:Use operator overload to create Operator
//	complicated pointer. Might need to overload the operator multiple time
//TODO: higher degree of derivative
//TODO: error handling
//TODO: initFunction can be called only one
//TODO: deconstructor
//
//TODO: implement and test MatMul then differentiate
//TODO: is taking derivative on Tensor called taking gradient instead?

int main(){
	Var<Matrix<double, 3, 2>> w;
	Var<Matrix<double, 2, 1>> x;

	Op<tns::MatMul, Matrix<double, 3, 1>, Matrix<double, 3,2>, Matrix<double, 2,1>> f(&w,&x);

	f.evaluate().print();
	Matrix<double, 3, 1> df({{1}, {1}, {1}});

	return 0;
}

