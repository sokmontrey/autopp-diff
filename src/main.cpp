#include <iostream>
#include <cmath>

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

		virtual T compute() { return this->_value; }
		virtual void computeD( T chain_value ) {};

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

		T compute() override {
			this->_d_value = T();
			return this->_value;
		}
		void computeD(T chain_value){
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
		Node<T>* _a;
		Node<T>* _b;
	public:
		Op(Node<T> *a, Node<T> *b=nullptr){
			this->_type = OPERATOR;

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
			this->_d_value = T();

			if(this->_b)
				this->_value = OP<T>::compute(_a->compute(), _b->compute());
			else
				this->_value = OP<T>::compute(_a->compute());

			return this->_value;
		}

		void computeD(T chain_value=1) override {
			this->_d_value += chain_value;

			if(_b){
				if(_a->getType() != CONSTANT){
					_a->computeD( chain_value * 
						OP<T>::computeD(true, _a->getValue(), _b->getValue()) 
					);
				}

				if(_b->getType() != CONSTANT){
					_b->computeD( chain_value * 
						OP<T>::computeD(false, _a->getValue(), _b->getValue())
					);
				}
			}else{
				if(_a->getType() != CONSTANT){
					_a->computeD( chain_value * 
						OP<T>::computeD(true, _a->getValue()) 
					);
				}
			}
		}
};

template <typename T>
struct Add{
	static T compute(T a, T b=T()){
		return a + b;
	}

	static T computeD(bool is_a, T a, T b=T()){
		return 1;
	}
};

template <typename T>
struct ReLU{
	static T compute(T a, T b=T()){
		return a > 0 ? a : 0;
	}
	static T computeD(bool is_a, T a, T b=T()){
		return a > 0 ? 1 : 0;
	}
};

template <typename T>
struct Mul{
	static T compute(T a, T b=T()){
		return a * b;
	}
	static T computeD(bool is_a, T a, T b=T()){
		return is_a ? b : a;
	}
};

template <typename T>
struct Pow{
	static T compute(T a, T b=T()){
		return std::pow(a, b);
	}
	static T computeD(bool is_a, T a, T b=T()){
		return b * std::pow(a, b-1);
	}
};

//TODO:new design:
//	Node
//		Var
//		Op
//		Const


int main(){
	Var<int> a = 2;
	Const<int> b = 2;
	Const<int> c = 5;

	Op<Pow, int> power(&a, &b);
	Op<Mul, int> multiply(&power, &c);

	multiply.compute();
	multiply.computeD();

	std::cout << a.getDValue() << "\n";
	
	return 0;
}

