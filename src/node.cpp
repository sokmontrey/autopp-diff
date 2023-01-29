#include "node.h"

template <typename OBJECT_TYPE>
template <typename... Args>
Node<OBJECT_TYPE>::Node(Args&&... args): _object(std::forward<Args>(args)...){ }

template <typename OBJECT_TYPE>
Node<OBJECT_TYPE>::Node(OBJECT_TYPE &predefined_object){
	this->_object = predefined_object;
}
template <typename OBJECT_TYPE>
OBJECT_TYPE& Node<OBJECT_TYPE>::getObject(){
	return this->_object;
}
template <typename OBJECT_TYPE>
OBJECT_TYPE& Node<OBJECT_TYPE>::getDerivativeObject(){
	return this->_derivative_object;
}
template <typename OBJECT_TYPE>
void Node<OBJECT_TYPE>::setObject(OBJECT_TYPE &object){
	this->_object = object;
}
template <typename OBJECT_TYPE>
void Node<OBJECT_TYPE>::operator=(OBJECT_TYPE &object){
	this->setObject(object);
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
