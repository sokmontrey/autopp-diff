#include <iostream>
#include <cmath>
#include "./node.cpp"

//TRIED:Use operator overload to create Operator
//	complicated pointer. Might need to overload the operator multiple time
//TODO: make data type into class instead.
//TODO: higher degree of derivative
//TODO: error handling

//TODO: convert to other type method
template <typename T>
class DataType{
	protected:
		T _value;
	public:
		virtual T getValue() const { return this->_value; }
		virtual void setValue(T new_value) { this->_value = new_value; }

		virtual T add(T other){
			return this->_value + other
		}
};

class Double: public DataType<double>{
	public:
		Double(double value){
			this->_value = value;
		}
};

int main(){
	Double a = 10;

	std::cout << a.getValue() << "\n";

	return 0;
}

