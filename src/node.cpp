#include "node.h"

using namespace Node;

/*___________NODE___________*/

template <typename T>
BaseNode<T>::BaseNode() = default;

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
NODE_TYPE BaseNode<T>::getType(){
	return this->_type;
}

/*___________VAR___________*/

template <typename T>
Var<T>::Var() = default;

template <typename T>
Var<T>::Var(T value){
	this->_type = VARIABLE;
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

/*___________CONST___________*/

template <typename T>
Const<T>::Const() = default;

template <typename T>
Const<T>::Const(T value){
	this->_type = CONSTANT;
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

/*___________OP___________*/

//Both arguments are normal object
//	OR there is only one argument that is a normal object
template <template <typename> class OP, typename T>
Op<OP,T>::Op(BaseNode<T>* a, BaseNode<T>* b) {
	this->_type = OPERATOR;

	this->_a = std::shared_ptr<BaseNode<T>>(a);
	this->_b = std::shared_ptr<BaseNode<T>>(b);
}

//One of the arguments is a temporary object
//	a is a temp object
//	OR there is only a and a is a temporary object
template <template <typename> class OP, typename T>
Op<OP, T>::Op(BaseNode<T>&& a, BaseNode<T>* b) {
	this->_type = OPERATOR;

	this->_a = std::make_shared<BaseNode<T>>(std::move(a));
	this->_b = std::shared_ptr<BaseNode<T>>(b);
}

//	b is a temp object
template <template <typename> class OP, typename T>
Op<OP, T>::Op(BaseNode<T>* a, BaseNode<T>&& b) {
	this->_type = OPERATOR;

	this->_a = std::shared_ptr<BaseNode<T>>(a);
	this->_b = std::make_shared<BaseNode<T>>(std::move(b));
}

//Both arguments are temporary
template <template <typename> class OP, typename T>
Op<OP, T>::Op(BaseNode<T>&& a, BaseNode<T>&& b) {
	this->_type = OPERATOR;

	this->_a = std::make_shared<BaseNode<T>>(std::move(a));
	this->_b = std::make_shared<BaseNode<T>>(std::move(b));
}

template <template <typename> class OP, typename T>
void Op<OP, T>::setValue(T new_value) {
	//throw: modifying Operator's value doesn't affect anything
	std::cout << "Warning: Op immut" << "\n";
}

template <template <typename> class OP, typename T>
void Op<OP, T>::operator=(T value) {
	this->setValue(value);
}

template <template <typename> class OP, typename T>
T Op<OP, T>::evaluate() {
	//reset derivative value because
	//	when taking the derivative _d_value will be += to chain_value
	this->_d_value = T();

	if(_b) 
		this->_value = OP<T>::evaluate(_a->evaluate(), _b->evaluate());
	else 
		this->_value = OP<T>::evaluate(_a->evaluate());

	return this->_value;
}

template <template <typename> class OP, typename T>
void Op<OP, T>::differentiate(T derivative_factor) {
	this->_d_value += derivative_factor;

	if(_b){
		if(_a->getType() != CONSTANT){
			_a->differentiate( derivative_factor * 
				OP<T>::differentiate(true, _a->getValue(), _b->getValue()) 
			);
		}

		if(_b->getType() != CONSTANT){
			_b->differentiate( derivative_factor * 
				OP<T>::differentiate(false, _a->getValue(), _b->getValue())
			);
		}
	}else{
		if(_a->getType() != CONSTANT){
			_a->differentiate( derivative_factor * 
				OP<T>::differentiate(true, _a->getValue()) 
			);
		}
	}
}
