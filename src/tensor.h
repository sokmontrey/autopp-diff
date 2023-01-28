#ifndef TENSOR_H
#define TENSOR_H

#include <iostream>
#include <random>
#include <algorithm>

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

		virtual void setValue(size_t index, T value);
		virtual void setValue(size_t (&&indexes)[DIMENSION], T value);

		virtual T getValue(size_t index=0) const;
		virtual T getValue(size_t (&&indexes)[DIMENSION]) const;

		void operator=(Tensor &other);

		size_t getTotalSize() const;

		//param: axis 0 is the outer most dimension axis
		size_t getSize(size_t axis) const;

		size_t getDimension() const;

		virtual void print() const;
		virtual void printShape() const;
};

template <typename T>
class Scalar: public Tensor<T, 1, 0>{
	private:
		size_t _indexing(size_t (&indexes)[0]) const override;
	public:
		Scalar();
		Scalar(T initial_value);

		T getValue() const;
		void setValue(T new_value);

		T& operator()();
		void operator=(T new_value);

		void print() const override;
		void printShape() const override;
};

template <typename T, size_t LENGTH>
class Vector: public Tensor<T, LENGTH, 1>{
    private:
        size_t _indexing(size_t (&indexes)[1]) const override;
    public:
        Vector();
        Vector(T (&&arr)[LENGTH]);
        Vector(T initial_value);
        Vector(double min_range, double max_range, double seed);
        T getValue(size_t (&&indexes)[1]) const override;
        size_t getLength() const;
        void setValue(size_t (&&indexes)[1], T value) override;
        T& operator()(size_t index);
        void operator=(Vector &other);
};

//TODO: getRow, getCol
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
		T getValue(size_t (&&indexes)[2]) const override;
		T getValue(size_t row, size_t col) const;
		void setValue(size_t (&&indexes)[2], T value) override;
		void setValue(size_t row, size_t col, T value);
		T& operator()(size_t row, size_t col);
		void operator=(Matrix &other);
		void print() const override;
};

#endif //TENSOR_H
