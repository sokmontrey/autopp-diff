#include <iostream>
#include <cmath>
#include "./node.cpp"

//TRIED:Use operator overload to create Operator
//	complicated pointer. Might need to overload the operator multiple time
//TODO: split code into organized files structure
//TODO: support other data type
//	such as custom matrix operation
//TODO: higher degree of derivative
//TODO: determine the safety of storing normal ptr to a shared_ptr
//TODO: error handling

int main(){
	Var<double> x = 5;
	Var<double> w = 2;
	Var<double> b = 3;
	
	Op<ReLU, double> f(
		new Op<Add, double>(
			new Op<Mul, double>(&x, &w),
			&b
		)
	);
	
	std::cout << f.evaluate() << "\n";

	f.differentiate();

	std::cout << w.getDValue() << "\n";

	return 0;
}

