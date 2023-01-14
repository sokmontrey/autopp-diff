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
		DataType(T value=T()): _value(value) {}

		virtual T getValue() const { return this->_value; }
		virtual void setValue(T new_value) { this->_value = new_value; }
		
        virtual DataType<T> operator+(const DataType<T> &other) const { 
			return DataType<T>(_value + other.getValue()); 
		}
        virtual DataType<T> operator-(const DataType<T> &other) const { 
			return DataType<T>(_value - other.getValue()); 
		}
        virtual DataType<T> operator*(const DataType<T> &other) const { 
			return DataType<T>(_value * other.getValue()); 
		}
        virtual DataType<T> operator/(const DataType<T> &other) const { 
			return DataType<T>(_value / other.getValue()); 
		}
};

class Double: public DataType<double>{
	public:
		Double(double value): DataType<double>(value){ }

		Double operator%(const Double &other) const {
			return Double(_value - other._value * std::trunc(_value/other._value));
		}
};

class Int: public DataType<int>{
	public:
		Int(int value): DataType<int>(value){ }
};

int main(){
	Double a(10);
	Double b(20);

	return 0;
}

