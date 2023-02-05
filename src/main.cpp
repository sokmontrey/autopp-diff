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

int main(){
	Var<Matrix<double, 2,3>> a;
	a.getTensor().initFromArray({{1,2,3}, {4,5,6}});
	Var<Matrix<double, 3,2>> b;
	b.getTensor().initFromArray({{1,2}, {3,4}, {5,6}});

	Op<MatMul, Matrix<double, 2,2>, Matrix<double, 2,3>, Matrix<double, 3,2>> f(&a, &b);

	f.evaluate().print();

	return 0;
}

