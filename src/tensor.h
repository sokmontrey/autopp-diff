#ifndef TENSOR_H
#define TENSOR_H

#include <iostream>
#include <random>
#include <algorithm>

/*
 * Tensor: A base class for creating n-dimensional array of data.
 * 		Tensor can be 1D (Scalar), 2D (Vector), 3D (Matrix), ..., ND.
 *
 * Tensor<typename T, 
 * 		size_t T_SIZE, 
 * 		size_t DIMENSION
 * 	>(
 * 		size_t (&&shape)[DIMENSION], //temporary array
 * 		bool is_define_indexing_multiplyer
 * 	);
 *
 * Params:
 * 		T: data type (int, double, long, short...)
 * 		T_SIZE: total size of elements in the tensor.
 * 			This can be calculated by multiply all the size of each axis
 * 			S1 * S2 * S3 * ... * Sn
 * 		DIMENSION: Total number of dimension or axis
 * 		shape: 1D array representing size of each axis
 * 		is_define_indexing_multiplyer: is TRUE by default. 
 * 			This tell the Tensor to calculate a general indexing_multiplyer for any n-dimension Tensor
 * 			Assign FALSE if extending Tensor while overloading the _indexing method for a better performence.
 *
 * Data is stored in as row-major 1D array.
 * ⎡ a b c ⎤ -> [a, b, c, d, e, f]
 * ⎣ d e f ⎦
 *
 * Shape is an 1D array of size_t that represent the size of each axis of the tensor.
 * 		From left to right, each element of shape corresponding to the size of each Axis (index) from the highest to the lowest.
 *
 * 		Example: [3 * 5 * 10]. In this case, the tensor is 3 dimension and the shape stated that there are 3 set of (5 set of 10 elements).
 *
 * 		NOTE: Axis 0 is the outer most (highest) axis.
 *
 */

template <typename T, size_t T_SIZE=1, size_t DIMENSION=1>
class Tensor{
	protected:
		T _data[T_SIZE] = {0};

		size_t _shape[DIMENSION];
		size_t _indexing_multiplyer[DIMENSION];

		virtual size_t _indexing(size_t (&indexes)[DIMENSION]) const;

	public:
		Tensor(size_t (&&shape)[DIMENSION] = {},
				bool is_define_indexing_multiplyer=true);

		void initDefault(T initial_value);
		void initRandom(double min_range, double max_range, double seed);

		/*----------Setter----------*/
		virtual void setValue(size_t index, T value);
		virtual void setValue(size_t (&&indexes)[DIMENSION], T value);

		/*----------Getter----------*/
		virtual T getValue(size_t index=0) const;
		virtual T getValue(size_t (&&indexes)[DIMENSION]) const;

		size_t getTotalSize() const;
		//param: axis 0 is the outer most dimension axis
		size_t getSize(size_t axis) const;
		size_t getDimension() const;


		/*----------Operator Overload----------*/
		void operator=(Tensor &other);

		/*----------Print----------*/
		virtual void print() const;
		virtual void printShape() const;
};

/*
 * Scalar: an extention of Tensor with 0 dimension
 * 		used for representing a single variable or constant.
 *
 * Scalar<typename T>(T initial_value);
 */
template <typename T>
class Scalar: public Tensor<T, 1, 0>{
	private:
		size_t _indexing(size_t (&indexes)[0]) const override;
	public:
		Scalar();
		Scalar(T initial_value);
		Scalar(double min_range, double max_range, double seed);

		/*----------Getter----------*/
		T getValue() const;

		/*----------Setter----------*/
		void setValue(T new_value);

		/*----------Operator Overload----------*/
		T& operator()();
		void operator=(T new_value);

		/*----------Print----------*/
		void print() const override;
		void printShape() const override;
};

/*
 * Vector: an extention of Tensor with 1 dimension
 * 		used for representing an array of data (vector)
 *
 * Vector<typename T, size_t LENGTH>(T initial_value);
 * 		create a vector and assign initial_value to every element
 * or
 * Vector<typename T, size_t LENGTH>(double min_range, double max_range, double seed);
 * 		create a vector and assign random values to each element
 * 		with the range from min_range to max_range and the seed of 1 as default
 *
 * or
 * Vector<typename T, size_t LENGTH>(T (&&arr)[LENGTH]); 
 * 		create a vector with a temporary array initiallization
 * 		and assign each element of the array to each element of the vector
 */
template <typename T, size_t LENGTH>
class Vector: public Tensor<T, LENGTH, 1>{
    private:
        size_t _indexing(size_t (&indexes)[1]) const override;
    public:
        Vector();
        Vector(T (&&arr)[LENGTH]);
        Vector(T initial_value);
        Vector(double min_range, double max_range, double seed=1);

		/*----------Getter----------*/
        T getValue(size_t (&&indexes)[1]) const override;
        size_t getLength() const;

		/*----------Setter----------*/
        void setValue(size_t (&&indexes)[1], T value) override;

		/*----------Operator Overload----------*/
        T& operator()(size_t index);
        void operator=(Vector &other);
};

/*
 * Matrix: an extention of Tensor with 2 dimension
 * 		used for representing a 2d array (matrix)
 *
 * Matrix<typename T, size_t ROWS, size_t COLS>(T initial_value);
 * 		create a matrix and assign initial_value to every element
 * or
 * Matrix<typename T, size_t ROWS, size_t COLS>(double min_range, double max_range, double seed);
 * 		create a matrix and assign random values to each element
 * 		with the range from min_range to max_range and the seed of 1 as default
 *
 * or
 * Matrix<typename T, size_t ROWS, size_t COLS>(T (&&arr)[ROWS][COLS]); 
 * 		create a matrix with a 2d temporary array initiallization
 * 		and assign each element of the array to each element of the matrix
 */
template <typename T, size_t ROWS, size_t COLS>
class Matrix: public Tensor<T, ROWS * COLS, 2>{
	private:
		size_t _indexing(size_t (&indexes)[2]) const override;
		size_t _indexing(size_t row, size_t col) const;
	public:
		Matrix();
		Matrix(T initial_value);
		Matrix(double min_range, double max_range, double seed);
		Matrix(T (&&arr)[ROWS][COLS]);

		/*----------Getter----------*/
		T getValue(size_t (&&indexes)[2]) const override;
		T getValue(size_t row, size_t col) const;
		size_t getRow() const;
		size_t getCol() const;

		/*----------Setter----------*/
		void setValue(size_t (&&indexes)[2], T value) override;
		void setValue(size_t row, size_t col, T value);

		/*----------Operator Overload----------*/
		T& operator()(size_t row, size_t col);
		void operator=(Matrix &other);

		/*----------Print----------*/
		void print() const override;
};

#endif //TENSOR_H
