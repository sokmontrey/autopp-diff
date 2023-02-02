#include "node.h"

/*------------------------------Node------------------------------*/
template <typename TENSOR_TYPE>
template <typename... Args>
Node<TENSOR_TYPE>::Node(Args&&... args): _object(std::forward<Args>(args)...){ }

template <typename TENSOR_TYPE>
Node<TENSOR_TYPE>::Node(TENSOR_TYPE&predefined_object){
	this->_object = predefined_object;
}

/*------------------------------Compute------------------------------*/
template <typename TENSOR_TYPE>
TENSOR_TYPE& Node<TENSOR_TYPE>::evaluate(){
	return this->_object;
}
template <typename TENSOR_TYPE>
void Node<TENSOR_TYPE>::_differentiate(TENSOR_TYPE &derivative_factor){
	std::cout << "NODE DIFFERENTIATE\n";
	//TODO;
}

/*------------------------------Getter------------------------------*/
template <typename TENSOR_TYPE>
TENSOR_TYPE& Node<TENSOR_TYPE>::getObject(){
	return this->_object;
}
template <typename TENSOR_TYPE>
TENSOR_TYPE& Node<TENSOR_TYPE>::getDerivativeObject(){
	return this->_derivative_object;
}
template <typename TENSOR_TYPE>
NODE_TYPE Node<TENSOR_TYPE>::getNodeType(){
	return this->_node_type;
}
/*------------------------------Setter------------------------------*/
template <typename TENSOR_TYPE>
void Node<TENSOR_TYPE>::setObject(TENSOR_TYPE &object){
	this->_object = object;
}
template <typename TENSOR_TYPE>
void Node<TENSOR_TYPE>::operator=(TENSOR_TYPE &object){
	this->setObject(object);
}

/*------------------------------Operator------------------------------*/

//Both arguments are normal object
//	OR there is only one argument that is a normal object
template <typename TENSOR_TYPE, typename FUNCTION>
Op<TENSOR_TYPE, FUNCTION>::Op(Node<TENSOR_TYPE> *node_a, Node<TENSOR_TYPE> *node_b){
	this->_node_a = std::shared_ptr<Node<TENSOR_TYPE>>(node_a);
	this->_node_b = std::shared_ptr<Node<TENSOR_TYPE>>(node_b);
}

//One of the arguments is a temporary object
//	a is a temp object
//	OR there is only a and a is a temporary object
template <typename TENSOR_TYPE, typename FUNCTION>
Op<TENSOR_TYPE, FUNCTION>::Op(Node<TENSOR_TYPE>&& node_a, Node<TENSOR_TYPE>* node_b){
	this->_node_a = std::make_shared<Node<TENSOR_TYPE>>(std::move(node_a));
	this->_node_b = std::shared_ptr<Node<TENSOR_TYPE>>(node_b);
}

//	b is a temp object
template <typename TENSOR_TYPE, typename FUNCTION>
Op<TENSOR_TYPE, FUNCTION>::Op(Node<TENSOR_TYPE>* node_a, Node<TENSOR_TYPE>&& node_b){
	this->_node_a = std::shared_ptr<Node<TENSOR_TYPE>>(node_a);
	this->_node_b = std::make_shared<Node<TENSOR_TYPE>>(std::move(node_b));
}

//Both arguments are temporary
template <typename TENSOR_TYPE, typename FUNCTION>
Op<TENSOR_TYPE, FUNCTION>::Op(Node<TENSOR_TYPE>&& node_a, Node<TENSOR_TYPE>&& node_b){
	this->_node_a = std::make_shared<Node<TENSOR_TYPE>>(std::move(node_a));
	this->_node_b = std::make_shared<Node<TENSOR_TYPE>>(std::move(node_b));
}
/*
template <typename T>
BaseNode<T>::BaseNode(NODE_TYPE node_type) {
	this->_node_type = node_type;
}

template <typename T>
void BaseNode<T>::setValue(T new_value){
	this->_value = new_value;
}

template <typename T>
void BaseNode<T>::operator=(T value){
	this->_value = value;
}

template <typename T>
T BaseNode<T>::evaluate(){
	return this->_value;
}

template <typename T>
void BaseNode<T>::differentiate(T derivative_factor){ }

template <typename T>
T BaseNode<T>::getValue(){
	return this->_value;
}
template <typename T>
T BaseNode<T>::getDValue(){
	return this->_d_value;
}

template <typename T>
NODE_TYPE BaseNode<T>::getNodeType(){
	return this->_node_type;
}

template <typename T>
Var<T>::Var():BaseNode<T>(VARIABLE) { }

template <typename T>
Var<T>::Var(T value):
	BaseNode<T>(VARIABLE){

	this->_value = value;
}

template <typename T>
T Var<T>::evaluate() {
	this->_d_value = T();
	return this->_value;
}

template <typename T>
void Var<T>::differentiate(T derivative_factor){
	this->_d_value += derivative_factor;
}

template <typename T>
Const<T>::Const() : BaseNode<T>(CONSTANT){ };

template <typename T>
Const<T>::Const(T value):BaseNode<T>(CONSTANT){
	this->_value = value;
}

template <typename T>
void Const<T>::setValue(T new_value) {
	//throw: Const is immutable
	std::cout << "Warning: Constant is immutable" << "\n";
}

template <typename T>
void Const<T>::operator=(T value) {
	this->setValue(value);
}

template <typename T>
void Const<T>::differentiate(T derivative_factor) {  } 

//Both arguments are normal object
//	OR there is only one argument that is a normal object
template <typename T, template <typename> class OP>
Op<T, OP>::Op(BaseNode<T>* a, BaseNode<T>* b):
	BaseNode<T>(OPERATOR) {

	this->_a = std::shared_ptr<BaseNode<T>>(a);
	this->_b = std::shared_ptr<BaseNode<T>>(b);
}

//One of the arguments is a temporary object
//	a is a temp object
//	OR there is only a and a is a temporary object
template <typename T, template <typename> class OP>
Op<T, OP>::Op(BaseNode<T>&& a, BaseNode<T>* b):
	BaseNode<T>(OPERATOR) {

	this->_a = std::make_shared<BaseNode<T>>(std::move(a));
	this->_b = std::shared_ptr<BaseNode<T>>(b);
}

//	b is a temp object
template <typename T, template <typename> class OP>
Op<T, OP>::Op(BaseNode<T>* a, BaseNode<T>&& b):
	BaseNode<T>(OPERATOR) {

	this->_a = std::shared_ptr<BaseNode<T>>(a);
	this->_b = std::make_shared<BaseNode<T>>(std::move(b));
}

//Both arguments are temporary
template <typename T, template <typename> class OP>
Op<T, OP>::Op(BaseNode<T>&& a, BaseNode<T>&& b):
	BaseNode<T>(OPERATOR) {

	this->_a = std::make_shared<BaseNode<T>>(std::move(a));
	this->_b = std::make_shared<BaseNode<T>>(std::move(b));
}

template <typename T, template <typename> class OP>
void Op<T, OP>::setValue(T new_value) {
	//throw: modifying Operator's value doesn't affect anything
	std::cout << "Warning: Op immut" << "\n";
}

template <typename T, template <typename> class OP>
void Op<T, OP>::operator=(T value) {
	this->setValue(value);
}

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
