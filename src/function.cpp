#include "./function.h"

using namespace nodeflow;

/*-------------------------------Flatten-----------------------------*/
template <typename TT, typename TA, typename TB>
void Flatten<TT,TA,TB>::evaluateTo(TT *to_be_assign, TA *a, TB *b){
	for(size_t i=0; i<to_be_assign->getTotalSize(); i++){
		to_be_assign->setValue(i, a->getValue(i));
	}
}
template <typename TT, typename TA, typename TB>
TT Flatten<TT,TA,TB>::evaluate(TA &a, TB &b){
	TT result;
	Flatten<TT,TA,TB>::evaluateTo(&result, &a, &b);
	return result;
}

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
/*-------------------------------Invert-----------------------------*/
template <typename TT, typename TA, typename TB>
void Invert<TT,TA,TB>::evaluateTo(TT *to_be_assign, TA *a, TB *b){
	for(size_t i=0; i<to_be_assign->getTotalSize(); i++){
		to_be_assign->setValue(i, -a->getValue(i));
	}
}
template <typename TT, typename TA, typename TB>
TT Invert<TT,TA,TB>::evaluate(TA &a){
	TT result;
	Invert<TT,TA,TB>::evaluateTo(&result, &a);
	return result;
}
template <typename TT, typename TA, typename TB>
void Invert<TT,TA,TB>::differentiateTo(TT *derivative_factor, 
		TA *to_be_assign_a, TB *to_be_assign_b,  
		TA *a, TB *b){

	for(size_t i=0; i<to_be_assign_a->getTotalSize(); i++){
		to_be_assign_a->setValue(i, -derivative_factor->getValue(i));
	}
}
/*-------------------------------Substract-----------------------------*/
template <typename TT, typename TA, typename TB>
void Substract<TT,TA,TB>::evaluateTo(TT *to_be_assign, TA *a, TB *b){
	for(size_t i=0; i<to_be_assign->getTotalSize(); i++){
		to_be_assign->setValue(i, a->getValue(i)-b->getValue(i));
	}
}
template <typename TT, typename TA, typename TB>
TT Substract<TT,TA,TB>::evaluate(TA &a, TB &b){
	TT result;
	Substract<TT,TA,TB>::evaluateTo(&result, &a, &b);
	return result;
}
template <typename TT, typename TA, typename TB>
void Substract<TT,TA,TB>::differentiateTo(TT *derivative_factor, 
		TA *to_be_assign_a, TB *to_be_assign_b,  
		TA *a, TB *b){

	for(size_t i=0; i<to_be_assign_a->getTotalSize(); i++){
		to_be_assign_a->setValue(i, derivative_factor->getValue(i));
		to_be_assign_b->setValue(i, -derivative_factor->getValue(i));
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
	
	Matrix<double, b->getCol(), b->getRow()> transpose_b(0);
	MatTranspose<Matrix<double, b->getCol(), b->getRow()>, TB>
		::evaluateTo(&transpose_b, b);
	MatMul<TA, TT, Matrix<double, b->getCol(), b->getRow()>>::evaluateTo(
			to_be_assign_a, 
			derivative_factor,
			&transpose_b);

	Matrix<double, a->getCol(), a->getRow()> transpose_a(0);
	MatTranspose<Matrix<double, a->getCol(), a->getRow()>, TA>
		::evaluateTo(&transpose_a, a);
	MatMul<TB,Matrix<double, a->getCol(), a->getRow()>, TT>::evaluateTo(
			to_be_assign_b, 
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
TT MatTranspose<TT,TA,TB>::evaluate(TA &a){
	TT result;
	MatTranspose<TT,TA,TB>::evaluateTo(&result, &a);
	return result;
}

/*-------------------------------Sum-----------------------------*/
template <typename TT, typename TA, typename TB>
void Sum<TT,TA,TB>::evaluateTo(TT *to_be_assign, TA *a, TB *b){
	to_be_assign->setValue(0,0);
	for(size_t i=0; i<a->getTotalSize(); i++){
		to_be_assign->setValue(0, to_be_assign->getValue(0) + a->getValue(i));
	}
}
template <typename TT, typename TA, typename TB>
TT Sum<TT,TA,TB>::evaluate(TA &a){
	TT result;
	Sum<TT,TA,TB>::evaluateTo(&result);
	return result;
}
template <typename TT, typename TA, typename TB>
void Sum<TT,TA,TB>::differentiateTo(TT *derivative_factor,
		TA *to_be_assign_a, TB *to_be_assign_b,  
		TA *a, TB *b){
	for(size_t i=0; i<to_be_assign_a->getTotalSize(); i++){
		to_be_assign_a->setValue(i, derivative_factor->getValue(0));
	}
}
/*-------------------------------ScalarPow-----------------------------*/
template <typename TT, typename TA, typename TB>
void ScalarPow<TT,TA,TB>::evaluateTo(TT *to_be_assign, TA *a, TB *b){
	for(size_t i=0; i<a->getTotalSize(); i++){
		to_be_assign->setValue(i, std::pow(a->getValue(i), b->getValue(0)));
	}
}
template <typename TT, typename TA, typename TB>
TT ScalarPow<TT,TA,TB>::evaluate(TA &a, TB &b){
	TT result;
	ScalarPow<TT,TA,TB>::evaluateTo(&result, &a, &b);
	return result;
}
template <typename TT, typename TA, typename TB>
void ScalarPow<TT,TA,TB>::differentiateTo(TT *derivative_factor,
		TA *to_be_assign_a, TB *to_be_assign_b,  
		TA *a, TB *b){
	for(size_t i=0; i<to_be_assign_a->getTotalSize(); i++){
		//n x^n-1
		to_be_assign_a->setValue(i, derivative_factor->getValue(i) 
				* b->getValue(0) 
				* std::pow(a->getValue(i), b->getValue(0)-1));
	}
}
/*-------------------------------ReLU-----------------------------*/
template <typename TT, typename TA, typename TB>
void ReLU<TT,TA,TB>::evaluateTo(TT *to_be_assign, TA *a, TB *b){
	for(size_t i=0; i<to_be_assign->getTotalSize(); i++){
		to_be_assign->setValue(i, a->getValue(i)>0? a->getValue(i) : 0);
	}
}
template <typename TT, typename TA, typename TB>
TT ReLU<TT,TA,TB>::evaluate(TA &a){
	TT result;
	ReLU<TT,TA,TB>::evaluateTo(&result, &a);
	return result;
}
template <typename TT, typename TA, typename TB>
void ReLU<TT,TA,TB>::differentiateTo(TT *derivative_factor,
		TA *to_be_assign_a, TB *to_be_assign_b,  
		TA *a, TB *b){
	for(size_t i=0; i<to_be_assign_a->getTotalSize(); i++){
		to_be_assign_a->setValue(i, a->getValue(i)>0? derivative_factor->getValue(i) : 0);
	}
}
/*-------------------------------Sigmoid-----------------------------*/
template <typename TT, typename TA, typename TB>
void Sigmoid<TT,TA,TB>::evaluateTo(TT *to_be_assign, TA *a, TB *b){
	for(size_t i=0; i<to_be_assign->getTotalSize(); i++){
		to_be_assign->setValue(i, 
			1 / (1 + std::exp(-a->getValue(i)))
		);
	}
}
template <typename TT, typename TA, typename TB>
TT Sigmoid<TT,TA,TB>::evaluate(TA &a){
	TT result;
	Sigmoid<TT,TA,TB>::evaluateTo(&result, &a);
	return result;
}
template <typename TT, typename TA, typename TB>
void Sigmoid<TT,TA,TB>::differentiateTo(TT *derivative_factor,
		TA *to_be_assign_a, TB *to_be_assign_b,  
		TA *a, TB *b){
	for(size_t i=0; i<to_be_assign_a->getTotalSize(); i++){
		double sigmoid = 1 / (1 + std::exp(-a->getValue(i)));
		to_be_assign_a->setValue(i, 
			sigmoid * (1 - sigmoid)
		);
	}
}
