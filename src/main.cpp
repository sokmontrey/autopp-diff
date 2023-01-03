#include <iostream>
#include <vector>

//PROBLEM: vector of _nodes are not a vector of references to nodes, but rather a vector of the copy of the nodes
template <typename T>
class Node {
	private:
		T _value;
		T _dValue;

		std::vector<Node<T>> _nodes;

		bool _isOperatorNode = false;

	public:
		Node() = default;
		Node(T value): _value(value) {}

		template <typename... Args>
		Node(Args... nodes){
			for(const auto& node: {nodes...}){
				this->_nodes.push_back(node);
			}
			this->_isOperatorNode = true;
		}

		T compute(){
			if(this->_isOperatorNode){
				this->_value = 1;
				for(int i=0; i<this->_nodes.size(); i++){
					this->_value *= this->_nodes[i].compute();
				}
			}
			this->_dValue = 0;
			return this->_value;
		}

		void derivative(T chain_value){
			if(this->_isOperatorNode){
				for(int i=0; i<this->_nodes.size(); i++){
					this->_nodes[i].derivative(chain_value * this->getValue() / this->_nodes[i].getValue());
				}
			}else{
				this->_dValue += chain_value;
			}
		}

		T getValue(){ return this->_value; }
		T getDValue(){return this->_dValue;}
};

int main(){
	Node<int> a = 10;
	Node<int> b = 20;
	Node<int> c = 30;
	Node<int> e(a, b, c);

	e.compute();

	e.derivative(1);

	std::cout << a.getDValue() << "\n";
	std::cout << b.getDValue() << "\n";

	return 0;
}
