#ifndef NODE_H
#define NODE_H

#include <memory>
#include <stdexcept>
#include <iostream>
#include "./function.cpp"

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
		TT _derivative_tensor;

	public:
		Node() = default;

		template <typename... Args>
		Node(Args&&... args);

		//create copy data from a predefinedtensor 
		Node(TT &predefined_tensor);

		/*------------------------------Compute------------------------------*/
		virtual TT& evaluate();
		virtual void differentiate(TT &derivative_factor);
		virtual void differentiate();

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
		void differentiate(TT &derivative_factor) override {}
		void differentiate() override {}
};

template <template <typename, typename, typename> class FUNCTION, 
	typename TT, typename TA=TT, typename TB=TT>
class Op: public Node<TT>{
	private:
		Node<TA> *_node_a;
		Node<TB> *_node_b;
	public:
		Op() = default;
		Op(Node<TA> *node_a, Node<TB> *node_b=nullptr);

		/*-------------------------------Compute-----------------------------*/
		TT& evaluate() override;
		void differentiate(TT &derivative_factor) override;
		void differentiate() override;
};

#endif //NODE_H
