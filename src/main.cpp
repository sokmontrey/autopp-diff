#include <iostream>
#include <cmath>
#include <memory>
#include <stdexcept>

enum NODE_TYPE{
	VARIABLE = 1,
	CONSTANT = 2,
	OPERATOR = 3,
};

//TODO: check type of T then assign if Premitive or Other (Matric, etc)
template <typename T>
class Node{
	protected:
		NODE_TYPE _type;

		T _value;
		T _d_value = T();

		Node() = default;

	public:

		virtual void setValue(T new_value){ this->_value = new_value; }
		virtual void operator=(T value){ this->_value = value; }

		virtual T evaluate() { return this->_value; }
		virtual void differentiate( T chain_value ) {};

		T getValue(){ return this->_value; }
		T getDValue(){ return this->_d_value; }
		NODE_TYPE getType(){ return this->_type; }
};


template <typename T>
class Var: public Node<T>{
	public:
		Var() = default;
		Var(T value){ 
			this->_type = VARIABLE;
			this->_value = value;
		}

		T evaluate() override {
			this->_d_value = T();
			return this->_value;
		}

		void differentiate(T chain_value){
			this->_d_value += chain_value;
		}
};

template <typename T>
class Const: public Node<T>{
	public:
		Const() = default;
		Const(T value){ 
			this->_type = CONSTANT;
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
		std::shared_ptr<Node<T>> _a;
		std::shared_ptr<Node<T>> _b;

	public:
		//Both arguments are normal object
		//	OR there is only one argument that is a normal object
		Op(Node<T>* a, Node<T>* b=nullptr){
			this->_type = OPERATOR;

			this->_a = std::shared_ptr<Node<T>>(a);
			this->_b = std::shared_ptr<Node<T>>(b);
		}

		//One of the arguments is a temporary object
		//	a is a temp object
		//	OR there is only a and a is a temporary object
		Op(Node<T>&& a, Node<T>* b=nullptr){
			this->_type = OPERATOR;

			this->_a = std::make_shared<Node<T>>(std::move(a));
			this->_b = std::shared_ptr<Node<T>>(b);
		}
		//	b is a temp object
		Op(Node<T>* a, Node<T>&& b){
			this->_type = OPERATOR;

			this->_a = std::shared_ptr<Node<T>>(a);
			this->_b = std::make_shared<Node<T>>(std::move(b));
		}

		//Both arguments are temporary
		Op(Node<T>&& a, Node<T>&& b){
			this->_type = OPERATOR;

			this->_a = std::make_shared<Node<T>>(std::move(a));
			this->_b = std::make_shared<Node<T>>(std::move(b));
		}


		void setValue(T new_value) override {
			//throw: modifying Operator's value doesn't affect anything
			std::cout << "Warning: Op immut" << "\n";
		}
		void operator=(T value) {
			this->setValue(value);
		}

		T evaluate() override {
			this->_d_value = T();

			if(this->_b)
				this->_value = OP<T>::evaluate(_a->evaluate(), _b->evaluate());
			else
				this->_value = OP<T>::evaluate(_a->evaluate());

			return this->_value;
		}

		void differentiate(T chain_value=1) override {
			this->_d_value += chain_value;

			if(_b){
				if(_a->getType() != CONSTANT){
					_a->differentiate( chain_value * 
						OP<T>::differentiate(true, _a->getValue(), _b->getValue()) 
					);
				}

				if(_b->getType() != CONSTANT){
					_b->differentiate( chain_value * 
						OP<T>::differentiate(false, _a->getValue(), _b->getValue())
					);
				}
			}else{
				if(_a->getType() != CONSTANT){
					_a->differentiate( chain_value * 
						OP<T>::differentiate(true, _a->getValue()) 
					);
				}
			}
		}
};

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

