#include <iostream>
#include <vector>
#include <cmath>

template <typename T>
class Node{
	private:
		T _value;
		T _d_value;

	public:
		Node() = default;
		Node(T value): _value(value) {}

		T getValue(){ return this->_value; }
		T getDValue(){ return this->_d_value; }

		void setDValue(T d_value){ this->_d_value = d_value; }
};

/*
template <template <typename> class OP, typename T>
class Node{
	private:
		Node<T> *_a = nullptr;
		Node<T> *_b = nullptr;

	public:
		Node() = default;
		Node(Node<T> *a): _a(a) { }
		Node(Node<T> *a, Node<T> *b): _a(a), _b(b) { }

		T compute(){
			if(_b)
				return OP<T>::compute( _a->getValue(), _b->getValue() );
			else
				return OP<T>::compute( _a->getValue() );
		}

		T computeD(bool is_a=true, bool is_update_node=true){
			//TODO: manage if the operator is single variable
			T d_value;

			if(_b)
				d_value = OP<T>::computeD( _a->getValue(), _b->getValue(), is_a );
			else
				d_value = OP<T>::computeD( _a->getValue(), is_a );

			if(is_update_node){
				is_a ? _a->setDValue(d_value) : _b->setDValue(d_value);
			}

			return d_value;
		}
};
*/

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
	
	return 0;
}

// Emery is good
// It is free

/* Dear Viewers,
Thank you for joining me on my YouTube LIVE Stream. I hope you have enjoyed the content and found it informative.

I would like to take this opportunity to say goodbye and express my gratitude for your support. It has been a pleasure interacting with you and sharing my knowledge and thoughts with you.

I hope you will continue to tune in for future streams and continue to support me in my endeavors.

Thank you again for joining me. I look forward to seeing you in the next stream.

Sincerely,
Sokmontrey Sythat */
