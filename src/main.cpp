#include <iostream>
#include <cmath>

template <typename T>
class Node{
	protected:
		T _value;
		T _d_value;

		Node() = default;

		virtual void setValue(T new_value){
			this->_value = new_value;
		}

		virtual void operator=(T value){
			this->_value = value;
		}

	public:
		virtual T compute() { return this->_value; }
		T getValue(){return this->_value;}
};


template <typename T>
class Var: public Node<T>{
	public:
		Var() = default;
		Var(T value){ 
			this->_value = value;
		}
};

template <typename T>
class Const: public Node<T>{
	public:
		Const(T value){ 
			this->_value = value;
		}

		void setValue (T new_value) override {
			//throw: Const is immutable
			std::cout << "Warning: Constant is immutable" << "\n";
		}
		void operator=(T value) override {
			this->setValue(value);
		}
};

template <template <typename> class OP, typename T>
class Op: public Node<T>{
	private:
		Node<T>* _a;
		Node<T>* _b;
	public:
		Op(Node<T> *a, Node<T> *b=nullptr){
			this->_a = a;
			this->_b = b;
		}

		void setValue(T new_value) override {
			//throw: modifying Operator's value doesn't affect anything
			std::cout << "Warning: Op immut" << "\n";
		}
		void operator=(T value) {
			this->setValue(value);
		}

		T compute() override {
			this->_value = OP<T>::compute(_a->compute(), _b->compute());
			return this->_value;
		}
};

template <typename T>
struct Add{
	static T compute(T a, T b=T()){
		return a + b;
	}

	static T computeD(T a, T b=T(), bool is_a=true){
		return 1;
	}
};

template <typename T>
struct ReLU{
	static T compute(T a){
		return a > 0 ? a : 0;
	}
	static T computeD(T a, bool is_a=true){
		return a > 0 ? 1 : 0;
	}
};

template <typename T>
struct Pow{
	static T compute(T a, T b=T()){
		return std::pow(a, b);
	}
	static T computeD(T a, T b=T(), bool is_a=true){
		return b * std::pow(a, b-1);
	}
};

//TODO:new design:
//	Node
//		Var
//		Op
//		Const


int main(){
	Var<int> a = 10;
	Const<int> b = 20;

	Op<Add, int> c(&a, &b);
	Op<Add, int> d(&c, &a);

	std::cout << a.compute() << "\n";
	std::cout << b.compute() << "\n";
	std::cout << c.compute() << "\n";
	std::cout << d.compute() << "\n";
	
	return 0;
}

