#include <iostream>
#include "./tensor.cpp"
#include "./node.cpp"
#include "./function.cpp"

//TRIED:Use operator overload to create Operator
//	complicated pointer. Might need to overload the operator multiple time
//TODO: higher degree of derivative
//TODO: error handling
//TODO: initFunction can be called only one
//TODO: deconstructor

//TODO: something is still going after Op. maybe because of there is no deconstructor

int main(){
	Var<Matrix<double, 2,3>> a(-1, 1, 0);
	Var<Matrix<double, 2,3>> b(-1, 1, 1);

	Op<Matrix<double, 2,3>, tns::Add<Matrix<double, 2,3>>> f(&a, &b);
	return 0;
}

