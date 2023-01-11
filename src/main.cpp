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
	Var<double> x = 5;
	Const<double> c = 2;

	Op<Sin, double> s(&x);
	Op<Pow, double> p(&x, &c);
	Op<Mul, double> m(&x, &c);
	Op<Add, double> a1(&p, &m);
	Op<Add, double> a2(&a1, &s);
	Op<Cos, double> f(&a2);

	std::cout << f.evaluate() << "\n";

	f.differentiate();

	std::cout << x.getDValue() << "\n";

	return 0;
}

