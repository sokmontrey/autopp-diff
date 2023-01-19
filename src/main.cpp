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
		T _data[T_SIZE] = {0};

		TENSOR_TYPE _tensor_type;
	public:
		Tensor(TENSOR_TYPE tensor_type): _tensor_type(tensor_type) { }

		virtual T getValue(size_t index=0) const { return this->_data[index]; }
		virtual T& operator()(size_t index=0) { return this->_data[index]; }
		virtual void setValue(T value) { this->_data[0] = value; }
		/*

		virtual void print() const {

		}

		Tensor operator+(Tensor &other){
			Tensor<T, T_SIZE> result;
			for(size_t i=0; i<T_SIZE; i++){
				result(i) = this->_data[i] + other.getValue(i);
			}
			return result;
		}
		*/
};

template <typename T>
class Scalar: public Tensor<T, 1>{
	public:
		Scalar(): Tensor<T, 1>(SCALAR) {}
		Scalar(T initial_value) : Tensor<T, 1>(SCALAR){
			this->_data[0] = initial_value;
		}

		T getValue() const override { return this->_data[0]; }
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

		Matrix(T (&&arr)[ROWS][COLS]): Tensor<T, ROWS*COLS>(MATRIX){
			for(size_t row=0; row<ROWS; row++){
				for(size_t col=0; col<COLS; col++){
					this->_data[index(row, col)] = arr[row][col];
				}
			}
		}

		//TODO: override getValue with no arguments
		T getValue(size_t row, size_t col) {
			return this->_data[index(row, col)];
		}
		T setValue(size_t row, size_t col, T value){
			this->_data[index(row, col)] = value;
		}

		T& operator()(size_t row, size_t col){
			return this->_data[index(row, col)];
		}

		/*
		void print(){
			std::cout << "\n";
			for(size_t row=0; row<ROWS; row++){
				for(size_t col=0; col<COLS; col++){
					std::cout << this->_data[index(row, col)] << " ";
				}
				std::cout << "\n";
			}
			std::cout << "\n";
		}
		*/

		size_t index(size_t row, size_t col){
			//column-major indexing
			return row * COLS + col;
		}
};

int main(){
	Matrix<double, 1, 4> a(2);

	Matrix<double, 1, 4> b(5);

	return 0;
}

