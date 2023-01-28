#include <iostream>
#include "./node.cpp"
#include "./tensor.cpp"

//TRIED:Use operator overload to create Operator
//	complicated pointer. Might need to overload the operator multiple time
//TODO: higher degree of derivative
//TODO: error handling

//TODO: make TENSOR even more general for n-dimension object
//TODO: getDimension
//

/*
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
					this->_data[i] = (T)( 
							(double)std::rand()/RAND_MAX*(max_range-min_range)+min_range
							);
			}
		}

		virtual void setValue(size_t index, T value){
			this->_data[index] = value;
		}
		virtual void setValue(size_t (&&indexes)[DIMENSION], T value){
			this->_data[this->_indexing(indexes)] = value;
		}

		virtual T getValue(size_t index=0) const {
			return this->_data[index];
		}
		virtual T getValue(size_t (&&indexes)[DIMENSION]) const {
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
		size_t getSize(size_t axis) const {
			return this->_shape[axis];
		}

		size_t getDimension() const {
			return DIMENSION;
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
		T getValue(size_t (&&indexes)[1]) const override {
			return this->_data[indexes[0]];
		}
		size_t getLength() {
			return this->getSize(0);
		}

		void setValue(size_t (&&indexes)[1], T value) override {
			this->_data[indexes[0]] = value;
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

template <typename T, size_t ROWS, size_t COLS>
class Matrix: public Tensor<T, ROWS * COLS, 2>{
	private:
		//sacrificed the generalization of being able to change the tensor 1d structure in memory
		//for performance
		size_t _indexing(size_t (&indexes)[2]) const override {
			return indexes[0] * ROWS + indexes[1];
		}
		size_t _indexing(size_t row, size_t col) const {
			return row * ROWS + col;
		}
	public:
		Matrix(): Tensor<T, ROWS * COLS, 2>({ROWS, COLS}, false) {}
		Matrix(T initial_value): Tensor<T, ROWS * COLS, 2>({ROWS, COLS}, false) {
			this->initDefault(initial_value);
		}
		Matrix(double min_range, double max_range, double seed)
		: Tensor<T, ROWS * COLS, 2>({ROWS, COLS}, false) {
			this->initRandom(min_range, max_range, seed);
		}
		Matrix(T (&&arr)[ROWS][COLS]): Tensor<T, ROWS*COLS, 2>({ROWS, COLS}, false){
			for(size_t row=0; row<ROWS; row++){
				for(size_t col=0; col<COLS; col++){
					this->_data[this->_indexing(row, col)] = arr[row][col];
				}
			}
		}
		T getValue(size_t (&&indexes)[2]) const override {
			return this->_data[this->_indexing(indexes)];
		}
		T getValue(size_t row, size_t col) const {
			return this->_data[this->_indexing(row, col)];
		}

		void setValue(size_t (&&indexes)[2], T value) override {
			this->_data[this->_indexing(indexes)] = value;
		}
		void setValue(size_t row, size_t col, T value) {
			this->_data[this->_indexing(row, col)] = value;
		}

		T& operator()(size_t row, size_t col){
			return this->_data[this->_indexing(row, col)];
		}

		void operator=(Matrix &other){
			//TODO: match size
			for(size_t i=0; i<this->getTotalSize; i++){
				this->_data[i] = other.getValue(i);
			}
		}
		void print() const override{
			std::cout << "\n";
			for(size_t row=0; row<ROWS; row++){
				for(size_t col=0; col<COLS; col++){
					std::cout << this->_data[this->_indexing(row, col)] << " ";
				}
				std::cout << "\n";
			}
		}
};
*/

int main(){
	Matrix<double, 2, 3> a({{1,2,3}, {4,5,6}});
	std::cout << a(1,1) << "\n";
	a.print();

	return 0;
}

