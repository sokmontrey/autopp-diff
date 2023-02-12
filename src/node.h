#ifndef NODE_H
#define NODE_H

#include <memory>
#include <stdexcept>
#include <iostream>
#include "./function.cpp"
#include "./tensor.cpp"

enum NODE_TYPE{
	VARIABLE = 1,
	CONSTANT = 2,
	OPERATOR = 3,
};

template <typename TT>
class Node{
	protected:
		NODE_TYPE _node_type = VARIABLE;

		TT _tensor;
		TT _derivative_tensor_a;
		TT _derivative_tensor_b;

	public:
		Node() = default;

		template <typename... Args>
		Node(Args&&... args);

		//create copy data from a predefinedtensor 
		Node(TT &predefined_tensor);

		/*------------------------------Compute------------------------------*/
		virtual TT& evaluate();
		virtual void differentiate(TT &derivative_factor);
		void differentiate();

		/*------------------------------Getter------------------------------*/
		TT& getTensor();
		virtual TT& getDerivativeTensor();
		NODE_TYPE getNodeType();

		/*------------------------------Setter------------------------------*/
		virtual void setTensor(TT&tensor);
		void operator=(TT&tensor);
};

template <typename TT>
class Var: public Node<TT>{
	using Node<TT>::Node;
	private:
		NODE_TYPE _node_type = VARIABLE;

	public:
		using Node<TT>::differentiate;
};

/*
 * The tensor in Constant Node can still be changed.
 * Const Node referred to the mathematical term, 
 * an tensorthat when take the derivative with respect to with is zero
 */
template <typename TT>
class Const: public Node<TT>{
	using Node<TT>::Node;
	private:
		NODE_TYPE _node_type = CONSTANT;
	public:
		using Node<TT>::differentiate;
};

template <template <typename, typename, typename> class FUNCTION, 
	typename TT, typename TA=TT, typename TB=TT>
class Op: public Node<TT>{
	private:
		Node<TA> *_node_a;
		Node<TB> *_node_b;
	public:
		//Nodes' differentiation method has no argument required
		//this is for when the differentiate is called with no argument
		using Node<TT>::differentiate;

		Op() = default;
		Op(Node<TA> *node_a, Node<TB> *node_b=nullptr);

		/*-------------------------------Compute-----------------------------*/
		TT& evaluate() override;
		void differentiate(TT &derivative_factor) override;
		
		/*-------------------------------Getter-----------------------------*/
		TT& getDerivativeTensorA();
		TT& getDerivativeTensorB();
};

#endif //NODE_H
