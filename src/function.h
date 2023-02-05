#ifndef OPERATOR_H
#define OPERATOR_H

#include <cmath>

//tns = tensor
namespace tns{

//TODO: match sizes
template <typename TT, typename TA=TT, typename TB=TT>
struct Add{
	static void evaluateTo(TT *to_be_assign, TA *a, TB *b=nullptr);
	static TT evaluate(TA &a, TB &b);
};

}

#endif //OPERATOR_H
