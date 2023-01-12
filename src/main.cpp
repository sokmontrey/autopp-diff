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
	Var<int> x = 10;
	Matrix<double, 2, 3> a(1);

	return 0;
}

