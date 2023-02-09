#include "./function.h"

using namespace tns;

/*-------------------------------Add-----------------------------*/
template <typename TT, typename TA, typename TB>
void Add<TT,TA,TB>::evaluateTo(TT *to_be_assign, TA *a, TB *b){
	for(size_t i=0; i<to_be_assign->getTotalSize(); i++){
		to_be_assign->setValue(i, a->getValue(i) + b->getValue(i));
	}
}
template <typename TT, typename TA, typename TB>
TT Add<TT,TA,TB>::evaluate(TA &a, TB &b){
	TT result;
	Add<TT,TA,TB>::evaluateTo(&result, &a, &b);
	return result;
}
template <typename TT, typename TA, typename TB>
void Add<TT,TA,TB>::differentiateTo(bool is_a, TT *to_be_assign, TA *a, TB *b){
	for(size_t i=0; i<to_be_assign->getTotalSize(); i++){
		to_be_assign->setValue(i, 1);
	}
}
/*-------------------------------Mul-----------------------------*/
template <typename TT, typename TA, typename TB>
void Mul<TT,TA,TB>::evaluateTo(TT *to_be_assign, TA *a, TB *b){
	for(size_t i=0; i<to_be_assign->getTotalSize(); i++){
		to_be_assign->setValue(i, a->getValue(i) * b->getValue(i));
	}
}
template <typename TT, typename TA, typename TB>
TT Mul<TT,TA,TB>::evaluate(TA &a, TB &b){
	TT result;
	Mul<TT,TA,TB>::evaluateTo(&result, &a, &b);
	return result;
}
template <typename TT, typename TA, typename TB>
void Mul<TT,TA,TB>::differentiateTo(bool is_a, TT *to_be_assign, TA *a, TB *b){
	//use if statement outside of for loop should improve in performence
	if(is_a){
		for(size_t i=0; i<to_be_assign->getTotalSize(); i++){
			to_be_assign->setValue(i, b->getValue(i));
		}
	}else{
		for(size_t i=0; i<to_be_assign->getTotalSize(); i++){
			to_be_assign->setValue(i, a->getValue(i));
		}
	}
}

/*-------------------------------MatMul-----------------------------*/
template <typename TT, typename TA, typename TB>
void MatMul<TT,TA,TB>::evaluateTo(TT *to_be_assign, TA *a, TB *b){
	for (int i = 0; i < a->getRow(); i++) {
		for (int j = 0; j < b->getCol(); j++) {
			for (int k = 0; k < a->getCol(); k++) {
				to_be_assign->setValue( i, j, 
					to_be_assign->getValue(i,j) + (a->getValue(i, k) * b->getValue(k,j))
				);
			}
		}
	}
}
template <typename TT, typename TA, typename TB>
TT MatMul<TT,TA,TB>::evaluate(TA &a, TB &b){
	TT result;
	MatMul<TT,TA,TB>::evaluateTo(&result, &a, &b);
	return result;
}
template <typename TT, typename TA, typename TB>
void MatMul<TT,TA,TB>::differentiateTo(bool is_a, TT *to_be_assign, TA *a, TB *b){
	//TODO:
}
