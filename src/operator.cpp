#include <cmath>

namespace Node{

template <typename T>
struct Add{
	static T evaluate(T a, T b=T()){
		return a + b;
	}

	static T differentiate(bool is_a, T a, T b=T()){
		return 1;
	}
};

}
