#include <iostream>
#include <vector>

template <template <typename> class OP, typename T>
class Operator;

template <typename T>
class Node;

template <typename T>
class Node {
	private:
		T _value;
		T _value_d;
	public:
		Node() = default;
		Node(T value): _value(value) {}

		T GetValue(){ return this->_value; }
		T GetDerivative(){ return this->_value_d; }
};

template <template <typename> class OP, typename T>
class Operator {
	private:
		Node<T> &_a;
		Node<T> &_b;
	public:
		Operator() = default;
		Operator(Node<T> &a, Node<T> &b): _a(a), _b(b) {}

		T Compute() {
			return OP<T>::Compute(this->GetValueA, this->GetValueB);
		}
};

template <typename T>
struct Add{
	static T Compute(T a, T b){
		return a + b;
	}
	static T Derivative(){
		return 1;
	}
};

int main(){
	Node<int> a = 10;
	Node<int> b = 20;

	Operator<Add, int> add(a, b);

	return 0;
}
