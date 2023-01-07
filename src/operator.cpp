#include <cmath>

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
