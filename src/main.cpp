#include <iostream>
#include <cmath>
#include <random>
#include "./node.cpp"

//TRIED:Use operator overload to create Operator
//	complicated pointer. Might need to overload the operator multiple time
//TODO: make data type into class instead.
//TODO: higher degree of derivative
//TODO: error handling

enum TENSOR_TYPE{
	SCALAR = 1,
	VECTOR = 2,
	MATRIX = 3
};

template <typename T, size_t T_SIZE>
class Tensor{
	protected:
		T _data[T_SIZE];

		TENSOR_TYPE _tensor_type;
	public:
		Tensor(TENSOR_TYPE tensor_type): _tensor_type(tensor_type) { }

		T getValue() const { return this->_data[0]; }
};

template <typename T>
class Scalar: public Tensor<T, 1>{
	public:
		Scalar(): Tensor<T, 1>(SCALAR) {}
		Scalar(T initial_value) : Tensor<T, 1>(SCALAR){
			this->_data[0] = initial_value;
		}

		T getValue() const { return this->_data[0]; }
};

template <typename T, size_t ROWS, size_t COLS>
class Matrix: public Tensor<T, ROWS * COLS>{
	public:
		Matrix(): Tensor<T, ROWS * COLS>(MATRIX) {}

		Matrix(T initial_value): Tensor<T, ROWS * COLS>(MATRIX) {
			for(size_t row=0; row<ROWS; row++){
				for(size_t col=0; col<COLS; col++){
					this->_data[index(row, col)] = initial_value;
				}
			}
		}

		Matrix(double min_random, double max_random, double seed)
		: Tensor<T, ROWS * COLS>(MATRIX) {
			std::srand(seed);
			for(size_t row=0; row<ROWS; row++){
				for(size_t col=0; col<COLS; col++){
					this->_data[index(row, col)] = (double)std::rand()/RAND_MAX*(max_random-min_random)+min_random;
				}
			}
		}

		T& getValue(size_t row, size_t col) {
			return this->_data[index(row, col)];
		}

		void print(){
			for(size_t row=0; row<ROWS; row++){
				for(size_t col=0; col<COLS; col++){
					std::cout << this->_data[index(row, col)] << " ";
				}
				std::cout << "\n";
			}
		}

		size_t index(size_t row, size_t col){
			//column-major indexing
			return row * COLS + col;
		}
};

int main(){
	Matrix<double, 1, 4> a(-5, 5, 0);
	a.print();

	return 0;
}

