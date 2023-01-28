#include <iostream>
#include "./node.cpp"
#include "./tensor.cpp"

//TRIED:Use operator overload to create Operator
//	complicated pointer. Might need to overload the operator multiple time
//TODO: higher degree of derivative
//TODO: error handling
//TODO: initFunction can be called only one

int main(){
	Matrix<double, 2, 3> a({{1,2,3}, {4,5,6}});
	std::cout << a(1,1) << "\n";
	a.print();
	a.printShape();

	Tensor<double, 4, 1> b({4});
	b.initRandom(-1, 1, 1);
	b.print();
	b.printShape();

	return 0;
}

