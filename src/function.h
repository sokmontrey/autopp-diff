#ifndef OPERATOR_H
#define OPERATOR_H

#include <cmath>
#include <iostream>
#include "./tensor.cpp"

//nodeflow
namespace nodeflow{

//TODO: match sizes
template <typename TT, typename TA=TT, typename TB=TT>
struct Add{
	static TT evaluate(TA &a, TB &b);
	static void evaluateTo(TT *to_be_assign, TA *a, TB *b=nullptr);
	static void differentiateTo(
			TT *derivative_factor,
			TA *to_be_assign_a,  TB *to_be_assign_b,
			TA *a, TB *b=nullptr);
};

template <typename TT, typename TA=TT, typename TB=TT>
struct Invert{
	static TT evaluate(TA &a);
	static void evaluateTo(TT *to_be_assign, TA *a, TB *b=nullptr);
	static void differentiateTo(
			TT *derivative_factor,
			TA *to_be_assign_a,  TB *to_be_assign_b,
			TA *a, TB *b=nullptr);
};

template <typename TT, typename TA=TT, typename TB=TT>
struct Substract{
	static TT evaluate(TA &a, TB &b);
	static void evaluateTo(TT *to_be_assign, TA *a, TB *b=nullptr);
	static void differentiateTo(
			TT *derivative_factor,
			TA *to_be_assign_a,  TB *to_be_assign_b,
			TA *a, TB *b=nullptr);
};

template <typename TT, typename TA=TT, typename TB=TT>
struct Mul{
	static TT evaluate(TA &a, TB &b);
	static void evaluateTo(TT *to_be_assign, TA *a, TB *b=nullptr);
	static void differentiateTo(
			TT *derivative_factor,
			TA *to_be_assign_a, TB *to_be_assign_b, 
			TA *a, TB *b=nullptr);
};

template <typename TT, typename TA=TT, typename TB=TA>
struct MatMul{
	static TT evaluate(TA &a, TB &b);
	static void evaluateTo(TT *to_be_assign, TA *a, TB *b=nullptr);
	static void differentiateTo(
			TT *derivative_factor,
			TA *to_be_assign_a, TB *to_be_assign_b, 
			TA *a, TB *b=nullptr);
};

template <typename TT, typename TA=TT, typename TB=TA>
struct MatTranspose{
	static TT evaluate(TA &a);
	static void evaluateTo(TT *to_be_assign, TA *a, TB *b=nullptr);
};

template <typename TT, typename TA=TT, typename TB=TA>
struct Sum{
	static TT evaluate(TA &a);
	static void evaluateTo(TT *to_be_assign, TA *a, TB *b=nullptr);
	static void differentiateTo(
			TT *derivative_factor,
			TA *to_be_assign_a, TB *to_be_assign_b, 
			TA *a, TB *b=nullptr);
};

template <typename TT, typename TA=TT, typename TB=TA>
struct ScalarPow{
	static TT evaluate(TA &a, TB &b);
	static void evaluateTo(TT *to_be_assign, TA *a, TB *b=nullptr);
	static void differentiateTo(
			TT *derivative_factor,
			TA *to_be_assign_a, TB *to_be_assign_b, 
			TA *a, TB *b=nullptr);
};

/*-------------------------------Activation-----------------------------*/
template <typename TT, typename TA=TT, typename TB=TA>
struct ReLU{
	static TT evaluate(TA &a);
	static void evaluateTo(TT *to_be_assign, TA *a, TB *b=nullptr);
	static void differentiateTo(
			TT *derivative_factor,
			TA *to_be_assign_a, TB *to_be_assign_b, 
			TA *a, TB *b=nullptr);
};
template <typename TT, typename TA=TT, typename TB=TA>
struct Sigmoid{
	static TT evaluate(TA &a);
	static void evaluateTo(TT *to_be_assign, TA *a, TB *b=nullptr);
	static void differentiateTo(
			TT *derivative_factor,
			TA *to_be_assign_a, TB *to_be_assign_b, 
			TA *a, TB *b=nullptr);
};

}

#endif //OPERATOR_H
