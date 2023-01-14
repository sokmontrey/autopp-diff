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
		DataType(DataType&& other)noexcept:DataType<double>(std::move(other._value)){ }

		virtual T getValue() const { return this->_value; }
		virtual T operator()() const { return this->_value; }

		virtual void setValue(T new_value) { 
			this->_value = new_value;
		}
		virtual void operator=(T new_value) {
			this->_value = new_value; 
		}
		virtual void operator=(const DataType& other) { 
			this->_value = other.getValue(); 
		}
		virtual void operator=(DataType&& other) {
			this->_value = std::move(other._value);
		}
};

class Double: public DataType<double>{
	public:
		Double(double value): DataType<double>(value){ }

		Double operator+(const DataType& other) const {
			return Double(this->_value + other.getValue());
		}
		Double operator-(const DataType& other) const {
			return Double(this->_value - other.getValue());
		}
		Double operator*(const DataType& other) const {
			return Double(this->_value * other.getValue());
		}
		Double operator/(const DataType& other) const {
			return Double(this->_value / other.getValue());
		}
};

class Int: public DataType<int>{
	public:
		Int(int value): DataType<int>(value){ }
};

/*
 *
 * row x col Column-major
 *
 *		col
 * row | a b |
 *	   | c d |
 */
template <typename T, std::size_t ROWS, std::size_t COLS>
class Matrix: public DataType<T>{
	private:
		T _data[ROWS * COLS];

		std::size_t _index(std::size_t row, std::size_t col){
			return col * ROWS + row;
		}

	public:
		Matrix() = default;
		Matrix(T initial_value){
			for(int row=0; row<ROWS; row++){
				for(int col=0; col<COLS; col++){
					this->_data[_index(row, col)] = initial_value;
				}
			}
		}

		T& getValue(std::size_t row, std::size_t col) const {
			return this->_data[_index(row, col)];
		}
		std::size_t getRowSize() const {
			return ROWS;
		}
		std::size_t getColSize() const {
			return COLS;
		}

		T& operator()(std::size_t row, std::size_t col) {
			return this->_data[_index(row, col)];
		}
};

int main(){
	Matrix<double, 8, 3> a(200);

	std::cout << a(7, 2) << "\n";

	return 0;
}

