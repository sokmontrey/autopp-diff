#include "node.h"

/*------------------------------Node------------------------------*/
template <typename TT>
template <typename... Args>
Node<TT>::Node(Args&&... args): _tensor(std::forward<Args>(args)...){ }

template <typename TT>
Node<TT>::Node(TT&predefined_tensor){
	this->_tensor= predefined_tensor;
}

/*Compute------------------------------*/
template <typename TT>
TT& Node<TT>::evaluate(){
	this->_derivative_tensor = TT();
	return this->_tensor;
}
template <typename TT>
void Node<TT>::differentiate(TT&derivative_factor){
	std::cout << "NODE DIFFERENTIATE\n";
	tns::Add<TT>::evaluateTo(
			&this->_tensor, 
			&this->_tensor, 
			&derivative_factor);
}

/*Getter------------------------------*/
template <typename TT>
TT& Node<TT>::getTensor(){
	return this->_tensor;
}
template <typename TT>
TT& Node<TT>::getDerivativeTensor(){
	return this->_derivative_tensor;
}
template <typename TT>
NODE_TYPE Node<TT>::getNodeType(){
	return this->_node_type;
}
/*Setter------------------------------*/
template <typename TT>
void Node<TT>::setTensor(TT&tensor){
	this->_tensor= tensor;
}
template <typename TT>
void Node<TT>::operator=(TT&tensor){
	this->setTensor(tensor);
}


/*-------------------------------Variable-----------------------------*/


/*Compute------------------------------*/
template <typename TT>
TT& Var<TT>::evaluate(){
	this->_derivative_tensor = TT();
	return this->_tensor;
}
template <typename TT>
void Var<TT>::differentiate(TT&derivative_factor){
	std::cout << "VAR DIFFERENTIATE\n";
	tns::Add<TT>::evaluateTo(
			&this->_tensor, 
			&this->_tensor, 
			&derivative_factor);
}

/*-------------------------------Constant-----------------------------*/

/*Compute-----------------------------*/
template <typename TT>
TT& Const<TT>::evaluate(){
	return this->_tensor;
}

template <typename TT>
void Const<TT>::differentiate(TT& derivative_factor){ }

/*------------------------------Operator------------------------------*/

template <template <typename, typename, typename> class FUNCTION,
	typename TT, typename TA, typename TB>
Op<FUNCTION, TT, TA, TB>
::Op(Node<TA> *node_a, Node<TB> *node_b)
: Node<TT>(){
	this->_node_a = node_a;
	this->_node_b = node_b;
}

/*Compute-----------------------------*/

template <template <typename, typename, typename> class FUNCTION,
	typename TT, typename TA, typename TB>
TT& Op<FUNCTION, TT, TA, TB>::evaluate(){
	this->_derivative_tensor = TT();

	if(this->_node_b){
		FUNCTION<TT, TA, TB>::evaluateTo(
			&this->_tensor,
			&this->_node_a->evaluate(),
			&this->_node_b->evaluate()
		);
	}else{
		FUNCTION<TT,TA,TB>::evaluateTo(
			&this->_tensor,
			&this->_node_a->evaluate()
		);
	}

	return this->_tensor;
}

/*

template <typename T, template <typename> class OP>
T Op<T, OP>::evaluate() {
	//reset derivative value because
	//	when taking the derivative _d_value will be += to chain_value
	this->_d_value = T();

	if(_b) 
		this->_value = OP<T>::evaluate(_a->evaluate(), _b->evaluate());
	else 
		this->_value = OP<T>::evaluate(_a->evaluate());

	return this->_value;
}

template <typename T, template <typename> class OP>
void Op<T, OP>::differentiate(T derivative_factor) {
	this->_d_value += derivative_factor;

	if(_b){
		if(_a->getNodeType() != CONSTANT){
			_a->differentiate( derivative_factor * 
				OP<T>::differentiate(true, _a->getValue(), _b->getValue()) 
			);
		}

		if(_b->getNodeType() != CONSTANT){
			_b->differentiate( derivative_factor * 
				OP<T>::differentiate(false, _a->getValue(), _b->getValue())
			);
		}
	}else{
		if(_a->getNodeType() != CONSTANT){
			_a->differentiate( derivative_factor * 
				OP<T>::differentiate(true, _a->getValue()) 
			);
		}
	}
}
*/
