#include <iostream>
#include <cmath>
#include "./node.cpp"

//TRIED:Use operator overload to create Operator
//	complicated pointer. Might need to overload the operator multiple time
//TODO: make data type into class instead.
//TODO: higher degree of derivative
//TODO: error handling

template <template <typename> class TENSOR_T, typename T>
class Tensor{
	public:
		Tensor() = default;

		virtual TENSOR_T<T> operator+(const TENSOR_T<T> &other) const {
			return TENSOR_T<T>(this->_value + other.getValue());
		}

		virtual TENSOR_T<double> toDouble() const {
			return TENSOR_T<double>(this->_value);
		} 
		virtual TENSOR_T<int> toInt() const {
			return TENSOR_T<int>(this->_value);
		}
};

template <typename T>
class Scalar: public Tensor<Scalar, T>{
	private:
		T _value;

	public:
		Scalar(T initial_value){
			this->_value = initial_value;
		}

		T getValue() const {
			return this->_value;
		}
		T operator()() const {
			return this->_value;
		}


};

int main(){

	Scalar<double> a = 29.19;
	Scalar<int> b = 20;

	std::cout << (a + b.toDouble())() << "\n";

	return 0;
}

