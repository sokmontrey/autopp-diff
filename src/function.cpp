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
