#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include "./node.cpp"

//TRIED:Use operator overload to create Operator
//	complicated pointer. Might need to overload the operator multiple time
//TODO: higher degree of derivative
//TODO: error handling


//TODO: make TENSOR even more general for n-dimension object

/*
 *
 * shape is 1D array that from right to left, 
 * represent the size from the largest to the smallest dimension respectively
 * Example: {4, 3, 10}
 * In this case, the Tensor is 3 dimension which has
 * 4 set of 3 set of 10 elements
 * Or in other word, 4 set of 3, and in each one there are 10 elements.
 *
 * axis 0 (zero) is the outer most dimension
 */

template <typename T, size_t T_SIZE=1, size_t DIMENSION=1>
class Tensor{
	protected:
		T _data[T_SIZE] = {0};

		size_t _shape[DIMENSION];
		size_t _indexing_multiplyer[DIMENSION];

		virtual size_t _indexing(size_t (&indexes)[DIMENSION]) const {
			size_t index = 0;
			for(size_t i=0; i<DIMENSION; i++){
				index += this->_indexing_multiplyer[i] * indexes[i];
			}
			return index;
		}

	public:
		Tensor(size_t (&&shape)[DIMENSION] = {},
				bool is_define_indexing_multiplyer=true){

			this->_shape[0] = shape[0];

			if(is_define_indexing_multiplyer){
				this->_indexing_multiplyer[DIMENSION-1] = 1;

				size_t last_indexing_multiplyer = 1;
				for(size_t axis=1; axis<DIMENSION; axis++){
					//transfer shape data from the argument to _shape 
					//while initiallizing index_multiplyer
					//to save iteration time
					this->_shape[axis] = shape[axis];

					//{ A*B*...*N-1, A*B*...*N-2, A*B, A }
					last_indexing_multiplyer *= shape[DIMENSION-axis];
					this->_indexing_multiplyer[DIMENSION-axis-1] = last_indexing_multiplyer;
				}
			}
		}

		void initDefault(T initial_value){
			std::fill(this->_data, this->_data + T_SIZE, initial_value);
		}
		void initRandom(double min_range, double max_range, double seed){
			std::srand(seed);
			for(size_t i=0; i<T_SIZE; i++){
					this->_data[i] = (T) ( 
							(double)std::rand()/ RAND_MAX*(max_range-min_range)+min_range
							);
			}
		}

		void setValue(size_t index, T value){
			this->_data[index] = value;
		}
		void setValue(size_t (&&indexes)[DIMENSION], T value){
			this->_data[this->_indexing(indexes)] = value;
		}

		T getValue(size_t index=0){
			return this->_data[index];
		}
		T getValue(size_t (&&indexes)[DIMENSION]){
			return this->getValue(this->_indexing(indexes));
		}

		void operator=(Tensor &other){
			//match T_SIZE
			for(size_t i=0; i<T_SIZE; i++){
				this->_data[i] = other.getValue(i);
			}
		}

		size_t getTotalSize() const {
			return T_SIZE;
		}

		//param: axis 0 is the outer most dimension axis
		size_t getSize(size_t axis){
			return this->_shape[axis];
		}

		virtual void print() const {
			std::cout << "\n";
			for(size_t i=0; i<T_SIZE; i++){
				std::cout << this->_data[i] << " ";
			}
			std::cout << "\n";
		}

		virtual void printShape() const {
			std::cout << "\n";
			for(size_t axis=0; axis<DIMENSION; axis++){
				std::cout << this->_shape[axis] << (axis < (DIMENSION-1) ? " * ": "");
			}
			std::cout << "\n";
		}
};

template <typename T>
class Scalar: public Tensor<T, 1, 0>{
	private:
		size_t _indexing(size_t (&indexes)[0]) const override {
			return 0;
		}
	public:
		Scalar(): Tensor<T, 1, 0>({}, false){}
		Scalar(T initial_value): Tensor<T, 1, 0>({}, false){
			this->_data[0] = initial_value;
		}

		T getValue() const { 
			return this->_data[0]; 
		}
		void setValue(T new_value) { 
			this->_data[0] = new_value; 
		}

		T& operator()() { 
			return this->_data[0]; 
		}
		void operator=(T new_value){ 
			this->_data[0] = new_value; 
		}

		void print() const override {
			std::cout << "\n" << this->_data[0] << "\n";
		}

		void printShape() const override {
			std::cout << "\n0\n";
		}
};

template <typename T, size_t LENGTH>
class Vector: public Tensor<T, LENGTH, 1>{
	private:
		size_t _indexing(size_t (&indexes)[1]) const override {
			return indexes[0];
		}
	public:
		Vector(): Tensor<T, LENGTH, 1>({LENGTH}, false){}
		//constructor for predefined vector value 
		Vector(T (&&arr)[LENGTH]): Tensor<T, LENGTH, 1>({LENGTH}, false){
			for(size_t i=0; i<LENGTH; i++){
				this->_data[i] = arr[i];
			}
		} 
		//constructor for initializing the whole vector to a common value
		Vector(T initial_value): Tensor<T, LENGTH, 1>({LENGTH}, false){
			this->initDefault(initial_value);
		}
		//contructor for assigning random number to each element of the vector
		Vector(double min_range, double max_range, double seed): Tensor<T, LENGTH, 1>({LENGTH}, false){
			this->initRandom(min_range, max_range, seed);
		}
		T getValue(size_t index) const {
			return this->_data[index];
		}
		size_t getLength() {
			return this->getSize(0);
		}
		void setValue(size_t index, T new_value){
			this->_data[index] = new_value;
		}

		T& operator()(size_t index){
			return this->_data[index];
		}
		void operator=(Vector &other){
			//TODO: match size
			for(size_t i=0; i<LENGTH; i++){
				this->_data[i] = other(i);
			}
		}
};
/*
template <typename T, size_t ROWS, size_t COLS>
class Matrix: public Tenor<T ROWS * COLS, 2>{
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

		size_t index(size_t row, size_t col){
			//column-major indexing
			return row * COLS + col;
		}
};
*/
int main(){
	Vector<double, 3> a(-10, 10, 1);
	a.print();

	a.printShape();

	std::cout << a.getLength() << "\n";

	return 0;
}

