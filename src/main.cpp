#include <iostream>
#include <cmath>
#include "./node.cpp"

#define L1 1
#define L2 10
#define L3 1

int main(){

	Var<Matrix<double, L1, 1>> l1;
	l1.getTensor()(0) = 1;

	Var<Matrix<double, L2, L1>> w1(-5,5,0);
	Var<Matrix<double, L3, L2>> w2(-5,5,0);

	Var<Matrix<double, L2, 1>> b1(-5,5,0);
	Var<Matrix<double, L3, 1>> b2(-5,5,0);

	Var<Matrix<double, L3, 1>> t;
	t.getTensor()(0) = 1;

	Op<MatMul, Matrix<double, L2, 1>, Matrix<double, L2, L1>, Matrix<double, L1, 1>> 
		mul1(&w1, &l1);
	Op<Add, Matrix<double, L2, 1>> 
		add1(&mul1, &b1);
	Op<ReLU, Matrix<double, L2, 1>> 
		l2(&add1);

	Op<MatMul, Matrix<double, L3, 1>, Matrix<double, L3, L2>, Matrix<double, L2, 1>> 
		mul2(&w2, &l2);
	Op<Add, Matrix<double, L3, 1>> 
		add2(&mul2, &b2);
	Op<ReLU, Matrix<double, L3, 1>> 
		model(&add2);

	Const<Matrix<double,1,1>> two(2);

	Op<Substract, Matrix<double, L3, 1>> 
		difference(&t, &model);
	Op<ScalarPow, Matrix<double, L3, 1>, Matrix<double, L3, 1>, Matrix<double,1,1>>
		squared(&difference, &two);
	Op<Sum, Matrix<double, 1,1>, Matrix<double, L3, 1>> 
		error(&squared);

	int set_size = 20;
	double train_data[set_size];
	double train_label[set_size];

	//create Dataset
	for(int i=0; i<set_size; i++){
		train_data[i] = i;
		train_label[i] = i + (double)std::rand()/RAND_MAX*2 - 1;
	}

	for(int i=0; i<set_size; i++){
		l1.getTensor()(0) = train_data[i];
		t.getTensor()(0) = train_label[i];

		error.evaluate().print();
		model.getTensor().print();
	}

	return 0;
}
