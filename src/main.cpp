#include <iostream>
#include <cmath>
#include "./node.cpp"
#include "./operator.cpp"

//TODO:Use operator overload to create Operator
//TODO: split code into organized files structure
//TODO: support other data type
//	such as custom matrix operation
//TODO: higher degree of derivative
//TODO: determine the safety of storing normal ptr to a shared_ptr
//TODO: error handling

int main(){
	Var<double> w = 10;
	Var<double> b = 2;
	Var<double> x = 3;

	Op<ReLU, double> f(
		new Op<Add, double>(
			new Op<Mul, double>(&w, &x), &b
		)
	);

	std::cout << f.evaluate() << "\n";
	f.differentiate();
	
	std::cout << w.getDValue() << "\n";
	return 0;
}

