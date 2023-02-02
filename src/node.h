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

template <typename TENSOR_TYPE>
class Node{
	protected:
		NODE_TYPE _node_type = VARIABLE;

		TENSOR_TYPE _tensor;
		TENSOR_TYPE _derivative_tensor;


	public:
		Node() = default;

		template <typename... Args>
		Node(Args&&... args);

		//create copy data from a predefinedtensor 
		Node(TENSOR_TYPE&predefined_tensor);

		/*------------------------------Compute------------------------------*/
		virtual TENSOR_TYPE& evaluate();
		virtual void differentiate(TENSOR_TYPE &derivative_factor);

		/*------------------------------Getter------------------------------*/
		TENSOR_TYPE& getTensor();
		virtual TENSOR_TYPE& getDerivativeTensor();
		NODE_TYPE getNodeType();

		/*------------------------------Setter------------------------------*/
		virtual void setTensor(TENSOR_TYPE &tensor);
		void operator=(TENSOR_TYPE&tensor);
};

template <typename TENSOR_TYPE>
class Var: public Node<TENSOR_TYPE>{
	using Node<TENSOR_TYPE>::Node;
	private:
		NODE_TYPE _node_type = VARIABLE;

		TENSOR_TYPE& evaluate() override;
		void differentiate(TENSOR_TYPE& derivative_factor) override;
};

/*
 * The tensor in Constant Node can still be changed.
 * Const Node referred to the mathematical term, 
 * an tensorthat when take the derivative with respect to with is zero
 */
template <typename TENSOR_TYPE>
class Const: public Node<TENSOR_TYPE>{
	using Node<TENSOR_TYPE>::Node;
	private:
		NODE_TYPE _node_type = CONSTANT;

	public:
		TENSOR_TYPE& evaluate() override;
		void differentiate(TENSOR_TYPE& derivative_factor) override;
};

template <typename TENSOR_TYPE, typename FUNCTION>
class Op: public Node<TENSOR_TYPE>{
	private:
		Node<TENSOR_TYPE> *_node_a;
		Node<TENSOR_TYPE> *_node_b;

	public:
		Op() = default;
		Op(Node<TENSOR_TYPE> *node_a, Node<TENSOR_TYPE> *node_b=nullptr);

		/*-------------------------------Compute-----------------------------*/
		TENSOR_TYPE& evaluate() override;
		void differentiate(TENSOR_TYPE& derivative_factor) override {}
};

#endif //NODE_H
