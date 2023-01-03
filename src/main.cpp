#include <iostream>
#include <vector>

template <typename T>
class Node {
private:
	T _value;
	T _d_value;

	Operator<T> _operator;
public:
	Node() = default;
	Node(T value): _value(value);
	Node(Opeator<T> &&operator): _operator(operator);
};

template <typename T>
class Operator{
private:
	Node<T> &_a;
	Node<T> &_b;

protected:
	void setA(Node<int> &a){ this->_a = a; }
	void setB(Node<int> &b){ this->_b = b; }

	T getValueA(){return this->_a.getValue();}
	T getValueB(){return this->_b.getValue();}

	T Compute();
	T Derivative();
}

template <typename T>
class Add: public Operator{
	Add(Node<T> &a, Node<T> &b): _a(a), _b(b) {}

	T Compute(){ return this->getValueA() + this->getValueB(); }
};


int main(){
	Node<int> a = 10;
	Node<int> b = 20;

	Node<int> c(Add(a, b));

	return 0;
}
