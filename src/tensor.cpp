#include "tensor.h"

/*----------Tensor----------*/

template <typename T, size_t T_SIZE, size_t DIMENSION>
size_t Tensor<T, T_SIZE, DIMENSION>::_indexing(size_t (&indexes)[DIMENSION]) const {
	size_t index = 0;
	for(size_t i=0; i<DIMENSION; i++){
		index += this->_indexing_multiplyer[i] * indexes[i];
	}
	return index;
}

template <typename T, size_t T_SIZE, size_t DIMENSION>
Tensor<T, T_SIZE, DIMENSION>::Tensor(size_t (&&shape)[DIMENSION], bool is_define_indexing_multiplyer){

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

template <typename T, size_t T_SIZE, size_t DIMENSION>
void Tensor<T, T_SIZE, DIMENSION>::initDefault(T initial_value){
	std::fill(this->_data, this->_data + T_SIZE, initial_value);
}
template <typename T, size_t T_SIZE, size_t DIMENSION>
void Tensor<T, T_SIZE, DIMENSION>::initRandom(double min_range, double max_range, double seed){
	std::srand(seed);
	for(size_t i=0; i<T_SIZE; i++){
			this->_data[i] = (T)( 
					(double)std::rand()/RAND_MAX*(max_range-min_range)+min_range
					);
	}
}

template <typename T, size_t T_SIZE, size_t DIMENSION>
void Tensor<T, T_SIZE, DIMENSION>::setValue(size_t index, T value){
	this->_data[index] = value;
}
template <typename T, size_t T_SIZE, size_t DIMENSION>
void Tensor<T, T_SIZE, DIMENSION>::setValue(size_t (&&indexes)[DIMENSION], T value){
	this->_data[this->_indexing(indexes)] = value;
}

template <typename T, size_t T_SIZE, size_t DIMENSION>
T Tensor<T, T_SIZE, DIMENSION>::getValue(size_t index) const {
	return this->_data[index];
}
template <typename T, size_t T_SIZE, size_t DIMENSION>
T Tensor<T, T_SIZE, DIMENSION>::getValue(size_t (&&indexes)[DIMENSION]) const {
	return this->getValue(this->_indexing(indexes));
}

template <typename T, size_t T_SIZE, size_t DIMENSION>
void Tensor<T, T_SIZE, DIMENSION>::operator=(Tensor &other){
	//match T_SIZE
	for(size_t i=0; i<T_SIZE; i++){
		this->_data[i] = other.getValue(i);
	}
}

template <typename T, size_t T_SIZE, size_t DIMENSION>
size_t Tensor<T, T_SIZE, DIMENSION>::getTotalSize() const {
	return T_SIZE;
}

//param: axis 0 is the outer most dimension axis
template <typename T, size_t T_SIZE, size_t DIMENSION>
size_t Tensor<T, T_SIZE, DIMENSION>::getSize(size_t axis) const {
	return this->_shape[axis];
}

template <typename T, size_t T_SIZE, size_t DIMENSION>
size_t Tensor<T, T_SIZE, DIMENSION>::getDimension() const {
	return DIMENSION;
}

template <typename T, size_t T_SIZE, size_t DIMENSION>
void Tensor<T, T_SIZE, DIMENSION>::print() const {
	std::cout << "\n";
	for(size_t i=0; i<T_SIZE; i++){
		std::cout << this->_data[i] << " ";
	}
	std::cout << "\n";
}

template <typename T, size_t T_SIZE, size_t DIMENSION>
void Tensor<T, T_SIZE, DIMENSION>::printShape() const {
	std::cout << "\n";
	for(size_t axis=0; axis<DIMENSION; axis++){
		std::cout << this->_shape[axis] << (axis < (DIMENSION-1) ? " * ": "");
	}
	std::cout << "\n";
}
/*----------Scalar----------*/

template <typename T>
size_t Scalar<T>::_indexing(size_t (&indexes)[0]) const {
	return 0;
}
template <typename T>
Scalar<T>::Scalar(): Tensor<T, 1, 0>({}, false){}
template <typename T>
Scalar<T>::Scalar(T initial_value): Tensor<T, 1, 0>({}, false){
	this->_data[0] = initial_value;
}

template <typename T>
T Scalar<T>::getValue() const { 
	return this->_data[0]; 
}
template <typename T>
void Scalar<T>::setValue(T new_value) { 
	this->_data[0] = new_value; 
}

template <typename T>
T& Scalar<T>::operator()() { 
	return this->_data[0]; 
}
template <typename T>
void Scalar<T>::operator=(T new_value){ 
	this->_data[0] = new_value; 
}

template <typename T>
void Scalar<T>::print() const {
	std::cout << "\n" << this->_data[0] << "\n";
}

template <typename T>
void Scalar<T>::printShape() const {
	std::cout << "\n0\n";
}

/*----------Vector----------*/

template <typename T, size_t LENGTH>
size_t Vector<T, LENGTH>::_indexing(size_t (&indexes)[1]) const {
	return indexes[0];
}
template <typename T, size_t LENGTH>
Vector<T, LENGTH>::Vector(): Tensor<T, LENGTH, 1>({LENGTH}, false){}
//for predefined vector value 
template <typename T, size_t LENGTH>
Vector<T, LENGTH>::Vector(T (&&arr)[LENGTH]): Tensor<T, LENGTH, 1>({LENGTH}, false){
	for(size_t i=0; i<LENGTH; i++){
		this->_data[i] = arr[i];
	}
} 
//for initializing the whole vector to a common value
template <typename T, size_t LENGTH>
Vector<T, LENGTH>::Vector(T initial_value): Tensor<T, LENGTH, 1>({LENGTH}, false){
	this->initDefault(initial_value);
}
//contructor for assigning random number to each element of the vector
template <typename T, size_t LENGTH>
Vector<T, LENGTH>::Vector(double min_range, double max_range, double seed): Tensor<T, LENGTH, 1>({LENGTH}, false){
	this->initRandom(min_range, max_range, seed);
}
template <typename T, size_t LENGTH>
T Vector<T, LENGTH>::getValue(size_t (&&indexes)[1]) const {
	return this->_data[indexes[0]];
}
template <typename T, size_t LENGTH>
size_t Vector<T, LENGTH>::getLength() const {
	return this->getSize(0);
}

template <typename T, size_t LENGTH>
void Vector<T, LENGTH>::setValue(size_t (&&indexes)[1], T value) {
	this->_data[indexes[0]] = value;
}

template <typename T, size_t LENGTH>
T& Vector<T, LENGTH>::operator()(size_t index){
	return this->_data[index];
}
template <typename T, size_t LENGTH>
void Vector<T, LENGTH>::operator=(Vector &other){
	//TODO: match size
	for(size_t i=0; i<LENGTH; i++){
		this->_data[i] = other(i);
	}
}

/*----------Matrix----------*/

template <typename T, size_t ROWS, size_t COLS>
//sacrificed the generalization of being able to change the tensor 1d structure in memory
//for performance
size_t Matrix<T, ROWS, COLS>::_indexing(size_t (&indexes)[2]) const {
	return indexes[0] * ROWS + indexes[1];
}
template <typename T, size_t ROWS, size_t COLS>
size_t Matrix<T, ROWS, COLS>::_indexing(size_t row, size_t col) const {
	return row * ROWS + col;
}
template <typename T, size_t ROWS, size_t COLS>
Matrix<T, ROWS, COLS>::Matrix(): Tensor<T, ROWS * COLS, 2>({ROWS, COLS}, false) {}
template <typename T, size_t ROWS, size_t COLS>
Matrix<T, ROWS, COLS>::Matrix(T initial_value): Tensor<T, ROWS * COLS, 2>({ROWS, COLS}, false) {
	this->initDefault(initial_value);
}
template <typename T, size_t ROWS, size_t COLS>
Matrix<T, ROWS, COLS>::Matrix(double min_range, double max_range, double seed)
: Tensor<T, ROWS * COLS, 2>({ROWS, COLS}, false) {
	this->initRandom(min_range, max_range, seed);
}
template <typename T, size_t ROWS, size_t COLS>
Matrix<T, ROWS, COLS>::Matrix(T (&&arr)[ROWS][COLS]): Tensor<T, ROWS*COLS, 2>({ROWS, COLS}, false){
	for(size_t row=0; row<ROWS; row++){
		for(size_t col=0; col<COLS; col++){
			this->_data[this->_indexing(row, col)] = arr[row][col];
		}
	}
}
template <typename T, size_t ROWS, size_t COLS>
T Matrix<T, ROWS, COLS>::getValue(size_t (&&indexes)[2]) const {
	return this->_data[this->_indexing(indexes)];
}
template <typename T, size_t ROWS, size_t COLS>
T Matrix<T, ROWS, COLS>::getValue(size_t row, size_t col) const {
	return this->_data[this->_indexing(row, col)];
}

template <typename T, size_t ROWS, size_t COLS>
size_t Matrix<T, ROWS, COLS>::getRow() const {
	return ROWS;
}

template <typename T, size_t ROWS, size_t COLS>
size_t Matrix<T, ROWS, COLS>::getCol() const {
	return COLS;
}

template <typename T, size_t ROWS, size_t COLS>
void Matrix<T, ROWS, COLS>::setValue(size_t (&&indexes)[2], T value) {
	this->_data[this->_indexing(indexes)] = value;
}
template <typename T, size_t ROWS, size_t COLS>
void Matrix<T, ROWS, COLS>::setValue(size_t row, size_t col, T value) {
	this->_data[this->_indexing(row, col)] = value;
}

template <typename T, size_t ROWS, size_t COLS>
T& Matrix<T, ROWS, COLS>::operator()(size_t row, size_t col){
	return this->_data[this->_indexing(row, col)];
}

template <typename T, size_t ROWS, size_t COLS>
void Matrix<T, ROWS, COLS>::operator=(Matrix &other){
	//TODO: match size
	for(size_t i=0; i<this->getTotalSize; i++){
		this->_data[i] = other.getValue(i);
	}
}
template <typename T, size_t ROWS, size_t COLS>
void Matrix<T, ROWS, COLS>::print() const {
	std::cout << "\n";
	for(size_t row=0; row<ROWS; row++){
		for(size_t col=0; col<COLS; col++){
			std::cout << this->_data[this->_indexing(row, col)] << " ";
		}
		std::cout << "\n";
	}
}
