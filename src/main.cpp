#include <iostream>
#include "./tensor.cpp"
#include "./node.cpp"

//TRIED:Use operator overload to create Operator
//	complicated pointer. Might need to overload the operator multiple time
//TODO: higher degree of derivative
//TODO: error handling
//TODO: initFunction can be called only one
//TODO: deconstructor

//TODO: something is still going after Op. maybe because of there is no deconstructor

int main(){
	Var<Matrix<double, 1,1>> a(2);
	Var<Matrix<double, 1,1>> b(3);
	Const<Matrix<double, 1,1>> c(20);

	return 0;
}

