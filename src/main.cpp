#include <iostream>
#include "./tensor.cpp"
#include "./node.cpp"

//TRIED:Use operator overload to create Operator
//	complicated pointer. Might need to overload the operator multiple time
//TODO: higher degree of derivative
//TODO: error handling
//TODO: initFunction can be called only one

int main(){
	Var<Matrix<double, 2,3>> a(-1, 1, 0);
	Var<Matrix<double, 2,3>> b(-1, 1, 1);

	return 0;
}

