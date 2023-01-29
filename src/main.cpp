#include <iostream>
#include "./tensor.cpp"

//TRIED:Use operator overload to create Operator
//	complicated pointer. Might need to overload the operator multiple time
//TODO: higher degree of derivative
//TODO: error handling
//TODO: initFunction can be called only one
//
//TODO: generalize all the Tensor constructor

template <typename OBJECT_TYPE>
class Node{
	protected:
		OBJECT_TYPE _object;
		OBJECT_TYPE _d_object;
	public:
		template <typename... Args>
		Node(Args&&... args): _object(std::forward<Args>(args)...) { }
};

int main(){
	Node<Matrix<double, 2, 3>> a_node(-1,1,1);
	Matrix<double, 2,3> b(1);
	Node<Matrix<double, 2, 3>> b_node(b);

	return 0;
}

