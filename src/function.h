#ifndef OPERATOR_H
#define OPERATOR_H

#include <cmath>
#include <iostream>
#include "./tensor.cpp"

//nodeflow
namespace nodeflow{

/*
 * NOTE: Create a custom Function struct
 * 1. Required template:
 * 		TT: Tensor Type (Type of tensor that the function will return)
 * 		TA: Tensor Type A, first input's Tensor Type
 * 		TB: Tensor Type B, first input's Tensor Type
 * 2. Every function will need to have 2 input, Tensor a and Tensor b, eventhough the function only operate on only one of the input.
 * 3. The function struct will need 
 * 		evaluateTo method in order to directly modify the Tensor with a mathematical function
 * 		differentiateTo method in order to take the derivative of the Tensor and modify a Tensor directly from its pointer
 * 		evaluate (optional) method that take reference and use the evaluateTo to return a new Tensor
 * 4. 
 */

//TODO: match sizes


//Flatten Function required a predefined vector size in order to correctly flat a matrix
//It will not automatically figure out the size for the output
template <typename TT, typename TA=TT, typename TB=TT>
struct Flatten{
	static TT evaluate(TA &a, TB &b);
	static void evaluateTo(TT *to_be_assign, TA *a, TB *b=nullptr);
};

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
