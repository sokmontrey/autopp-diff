#ifndef NODE_H
#define NODE_H

#include <memory>
#include <stdexcept>
#include <iostream>

enum NODE_TYPE{
	VARIABLE = 1,
	CONSTANT = 2,
	OPERATOR = 3,
};

template <typename TENSOR_TYPE>
class Node{
	protected:
		NODE_TYPE _node_type = VARIABLE;

		TENSOR_TYPE _object;
		TENSOR_TYPE _derivative_object;

		virtual void _differentiate(TENSOR_TYPE&derivative_factor);

	public:
		Node() = default;

		template <typename... Args>
		Node(Args&&... args);

		//create copy data from a predefined object
		Node(TENSOR_TYPE&predefined_object);

		/*------------------------------Compute------------------------------*/
		virtual TENSOR_TYPE& evaluate();

		/*------------------------------Getter------------------------------*/
		TENSOR_TYPE& getObject();
		virtual TENSOR_TYPE& getDerivativeObject();
		NODE_TYPE getNodeType();

		/*------------------------------Setter------------------------------*/
		virtual void setObject(TENSOR_TYPE&object);
		void operator=(TENSOR_TYPE&object);
};

template <typename TENSOR_TYPE>
class Var: public Node<TENSOR_TYPE>{
	using Node<TENSOR_TYPE>::Node;
	private:
		NODE_TYPE _node_type = VARIABLE;
};

/*
 * The object in Constant Node can still be change.
 * Const Node referred to the mathematical term, 
 * an object that when take the derivative with respect to with is zero
 */
template <typename TENSOR_TYPE>
class Const: public Node<TENSOR_TYPE>{
	using Node<TENSOR_TYPE>::Node;
	private:
		NODE_TYPE _node_type = CONSTANT;
};

template <typename TENSOR_TYPE, typename FUNCTION>
class Op: public Node<TENSOR_TYPE>{
	private:
		std::shared_ptr<Node<TENSOR_TYPE>> _node_a;
		std::shared_ptr<Node<TENSOR_TYPE>> _node_b;

	public:
		//Both arguments are normal object
		//	OR there is only one argument that is a normal object
		Op(Node<TENSOR_TYPE> *node_a, Node<TENSOR_TYPE> *node_b=nullptr);

		//One of the arguments is a temporary object
		//	a is a temp object
		//	OR there is only a and a is a temporary object
		Op(Node<TENSOR_TYPE> &&node_a,Node<TENSOR_TYPE> *node_b=nullptr);

		//	b is a temp object
		Op(Node<TENSOR_TYPE> *node_a, Node<TENSOR_TYPE> &&node_b);

		//Both arguments are temporary
		Op(Node<TENSOR_TYPE>&&node_a, Node<TENSOR_TYPE> &&node_b);
};

/*
template <typename T>
class Var: public BaseNode<T>{
	public:
		Var();
		Var(T value);

		T evaluate() override ;

		void differentiate(T derivative_factor=1);
};

template <typename T>
class Const: public BaseNode<T>{
	public:
		Const();
		Const(T value);

		void setValue (T new_value) override;
		void operator=(T value) override;

		void differentiate(T derivative_factor=1) override ;
};

template <typename T, template <typename> class OP>
class Op: public BaseNode<T>{
	private:
		std::shared_ptr<BaseNode<T>> _a;
		std::shared_ptr<BaseNode<T>> _b;

	public:
		//Both arguments are normal object
		//	OR there is only one argument that is a normal object
		Op(BaseNode<T>* a, BaseNode<T>* b=nullptr);

		//One of the arguments is a temporary object
		//	a is a temp object
		//	OR there is only a and a is a temporary object
		Op(BaseNode<T>&& a, BaseNode<T>* b=nullptr);

		//	b is a temp object
		Op(BaseNode<T>* a, BaseNode<T>&& b);

		//Both arguments are temporary
		Op(BaseNode<T>&& a, BaseNode<T>&& b);

		void setValue(T new_value) override;
		void operator=(T value);

		T evaluate() override;

		void differentiate(T derivative_factor=1) override;
};
*/
#endif //NODE_H
