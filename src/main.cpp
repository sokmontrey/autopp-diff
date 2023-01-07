#include <iostream>
#include <cmath>
#include "./node.cpp"

template <typename T>
struct Add{
	static T evaluate(T a, T b=T()){
		return a + b;
	}

	static T differentiate(bool is_a, T a, T b=T()){
		return 1;
	}
};

template <typename T>
struct ReLU{
	static T evaluate(T a, T b=T()){
		return a > 0 ? a : 0;
	}
	static T differentiate(bool is_a, T a, T b=T()){
		return a > 0 ? 1 : 0;
	}
};

template <typename T>
struct Mul{
	static T evaluate(T a, T b=T()){
		return a * b;
	}
	static T differentiate(bool is_a, T a, T b=T()){
		return is_a ? b : a;
	}
};

template <typename T>
struct Pow{
	static T evaluate(T a, T b=T()){
		return std::pow(a, b);
	}
	static T differentiate(bool is_a, T a, T b=T()){
		return b * std::pow(a, b-1);
	}
};

template <typename T>
struct Cos{
	static T evaluate(T a, T b=T()){
		return std::cos(a);
	}
	static T differentiate(bool is_a, T a, T b=T()){
		return -std::sin(a);
	}
};

template <typename T>
struct Sin{
	static T evaluate(T a, T b=T()){
		return std::sin(a);
	}
	static T differentiate(bool is_a, T a, T b=T()){
		return std::cos(a);
	}
};

//TODO:Use operator overload to create Operator
//TODO: split code into organized files structure
//TODO: support other data type
//	such as custom matrix operation
//TODO: higher degree of derivative
//TODO: determine the safety of storing normal ptr to a shared_ptr

int main(){
	Var<double> w = 1;
	Var<double> b = 2;
	Var<double> x = 3;

	Op<ReLU, double> f(
		new Op<Add, double>(
			new Op<Mul, double>(&w, &x), &b
		)
	);

	std::cout << f.evaluate() << "\n";
	f.differentiate();
	
	std::cout << w.getDValue() << "\n";
	return 0;
}

