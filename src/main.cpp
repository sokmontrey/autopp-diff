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
	Var<Matrix<double, 2, 3>> a;
	a.getTensor().initFromArray({{1,2,3}, {4,5,6}});
	Var<Matrix<double, 2, 3>> b;
	b.getTensor().initFromArray({{5,6,7}, {8,9,10}});

	Op<Mul, Matrix<double, 2, 3>> f(&a, &b);

	f.evaluate().print();

	f.differentiate();
	b.getDerivativeTensor().print();

	return 0;
}

