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
		T _d_value;
	public:
		Node() = default;
		Node(T value): _value(value) {}

		T GetValue(){ return this->_value; }
		T GetDValue(){ return this->_d_value; }

		void SetDValue(T d_value){ this->_d_value = d_value; }
};

template <template <typename> class OP, typename T>
class Operator {
	private:
		Node<T> &_a = temp;
		Node<T> &_b;
		
		bool _is_only_a = false;
	public:
		Operator() = default;
		Operator(Node<T> &a): _a(a) {
			this->_is_only_a = true;
		}
		Operator(Node<T> &a, Node<T> &b): _a(a), _b(b) {}

		T Compute() {
			if(this->_is_only_a)
				return OP<T>::Compute( this->_a.GetValue() );
			else
				return OP<T>::Compute( this->_a.GetValue(), this->_b.GetValue() );
		}

		T ComputeD(bool is_a=true, bool is_update_node=true){
			T d_value;
			if(this->_is_only_a){
				d_value = OP<T>::ComputeD( 
						this->_a.GetValue(), 
						is_a );
			}else{
				d_value = OP<T>::ComputeD( 
						this->_a.GetValue(), 
						this->_b.GetValue(), 
						is_a );
			}

			if( is_update_node ){
				if(is_a) this->_a.SetDValue(d_value);
				else this->_b.SetDValue(d_value);
			}

			return d_value;
		}
};

template <typename T>
struct Add{
	static T Compute(T a, T b){
		return a + b;
	}
	static T ComputeD(T a, T b, bool is_a = true){
		return 1;
	}
};

template <typename T>
struct Mul{
	static T Compute(T a, T b){
		return a * b;
	}
	static T ComputeD(T a, T b, bool isA = true){
		return isA ? b : a;
	}
};

//TODO: Pow function with custom parameter
template <typename T>
struct Pow{
	static T Compute(T a){
		return a * a;
	};
	static T ComputeD(T a){
		return 2 * a;
	}
};

int main(){
	Node<int> a = 10;
	Node<int> b = 20;

	Operator<Pow, int> c(a);
	std::cout << c.Compute() << "\n";

	return 0;
}
