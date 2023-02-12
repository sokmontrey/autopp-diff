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
void Add<TT,TA,TB>::differentiateTo(TT *derivative_factor, 
		TA *to_be_assign_a, TB *to_be_assign_b,  
		TA *a, TB *b){

	for(size_t i=0; i<to_be_assign_a->getTotalSize(); i++){
		//1 * derivative_factor
		to_be_assign_a->setValue(i, derivative_factor->getValue(i));
		to_be_assign_b->setValue(i, derivative_factor->getValue(i));
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
void Mul<TT,TA,TB>::differentiateTo(TT *derivative_factor,
		TA *to_be_assign_a, TB *to_be_assign_b,
		TA *a, TB *b){
	for(size_t i=0; i<to_be_assign_a->getTotalSize(); i++){
		to_be_assign_a->setValue(i, b->getValue(i) * derivative_factor->getValue(i));
		to_be_assign_b->setValue(i, a->getValue(i) * derivative_factor->getValue(i));
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
void MatMul<TT,TA,TB>::differentiateTo(TT *derivative_factor,
		TA *to_be_assign_a, TB *to_be_assign_b,  
		TA *a, TB *b){
	//TODO: formula: 
	//	respect to A: MatMul(C, Transpose(B))
	//	respect to B: MatMul(Transpose(A), C)
	//
	
	Matrix<double, b->getCol(), b->getRow()> transpose_b(0);
	tns::MatTranspose<Matrix<double, b->getCol(), b->getRow()>, TB>
		::evaluateTo(&transpose_b, b);
	TA temp_a;
	tns::MatMul<TA, TT, Matrix<double, b->getCol(), b->getRow()>>::evaluateTo(
			&temp_a, 
			derivative_factor,
			&transpose_b);

	Matrix<double, a->getCol(), a->getRow()> transpose_a(0);
	tns::MatTranspose<Matrix<double, a->getCol(), a->getRow()>, TA>
		::evaluateTo(&transpose_a, a);
	TB temp_b;
	tns::MatMul<TB,Matrix<double, a->getCol(), a->getRow()>, TT>::evaluateTo(
			&temp_b, 
			&transpose_a,
			derivative_factor);
}

/*-------------------------------MatTranspose-----------------------------*/
template <typename TT, typename TA, typename TB>
void MatTranspose<TT,TA,TB>::evaluateTo(TT *to_be_assign, TA *a, TB *b){
	for(size_t r=0; r<a->getRow(); r++){
		for(size_t c=0; c<a->getCol(); c++){
			to_be_assign->setValue(c, r, a->getValue(r, c)); 
		}
	}
}
template <typename TT, typename TA, typename TB>
TT MatTranspose<TT,TA,TB>::evaluate(TA &a, TB &b){
	TT result;
	MatTranspose<TT,TA,TB>::evaluateTo(&result, &a, &b);
	return result;
}

