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

#define L1 Matrix<double, 2, 1>
#define W1 Matrix<double, 3, 2>
#define L2 Matrix<double, 3, 1>

int main(){
	L1 input({{1},{2}});
	W1 weight({{1,2},{3,4},{5,6}});
	L2 bias({{1},{2},{3}});

	Var<L1> x(input);
	Var<W1> w(weight);
	Var<L2> b(bias);

	Op<tns::MatMul, L2, W1, L1> matmul(&w,&x);
	Op<tns::Add, L2> add(&matmul, &b);
	Op<tns::ReLU, L2> relu(&add);
	Op<tns::Sum, Matrix<double, 1, 1>, L2> sum(&relu);

	sum.evaluate().print();
	sum.differentiate();

	w.getDerivativeTensor().print();

	return 0;
}

