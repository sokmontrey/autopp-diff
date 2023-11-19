#pragma once

//TODO: finish all the derivative doc
// Implement:
//      Normal operator
//      Statistic operatos
//      Transpose operator

/**
 * NOTE: @params: is referring to the constructor parameter of the OperatorNode
 *      Constructor argument can be individual individual argument form: OPERATOR_NODE_NAME a(&a, &b, ...);
 *      OR list argument form: OPERATOR_NODE_NAME a{&a, &b, ...};
 *      The number of argument is depend on the type of operator node
 */

/**
 * Below is a template for creating a custom operator node, extensions of OperatorNode with different compute and derivative function
 *
class FunctionName: public OperatorNode<[number of input]>{
    //default constructor (check Pow for custom constructor)
    using OperatorNode<[number of input]>::OperatorNode;

    void compute() override {
        //Input Node(s) can be access by this->getInput(INPUT_INDEX)
        //      or this->getInput() for a signal input op 
        //      (if too lazy to use 0 index)
        //Result from the calculation MUST be assigned to this->value
        //EXAMPLE: this->value = this->getInputs(0) + this->getInputs(1);
    }

    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        //MUST: return a matrix as the result to the taking the partial derivative on the function with respect to this->inputs[input_wrt_index]
        //MUST: use this->outer_derivative according to chain rule
        //EXAMPLE: return this->outer_derivative;
        //      return outer_derivative directly since the partial derivative with respect to any of the input
        //      is 1 matrix. And 1 * outer_deivative is the outer_derivative itself.
    }
};
 *
 * The number of rows and cols of a Node is very important
 *      OperatorNode will automatically set the rows and cols to the rows and cols of the first input node.
 *      If an OperatorNode has a different number of rows and cols, for example matrix multiplication operation,
 *      Use:
            void setSize() override {
                this->setRows( this->inputs[0]->getRows() );
                this->setCols( this->inputs[1]->getCols() );
            }
        To override the number of rows and cols
 *
*/

namespace nodeflow{

/**
 * Since scalar, vector, and matrix can be represented using an Eigen::Matrix, 
 *      This doc will only refer to matrix. But the operator shoul work for 
 *      any other type of tensor with dimension smaller than the matrix.
 */

//==========================================================================
//                            Basic Function 
//==========================================================================

/**
 * Addition operator : a[i][j] + b[i][j]
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *      ∂b = 1
 *
 * @params: a: Node*: pointer to first input Node
 *          b: Node*: pointer to second input Node
*/
class Add: public OperatorNode<2>{
    using OperatorNode<2>::OperatorNode;

    void compute() override {
        this->value =
            this->getInput(0)
            +
            this->getInput(1)
        ;
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->outer_derivative;
    }
};

/**
 * Matrix multiplication operator a * b
 * TYPE: NON ELEMENT WISE
 *
 * DERIVATIVE: Jacobian matrix is multiplied directly with outer derivative directly to get sum of partial derivative
 *      ∂a = b.T * outer_derivative 
 *      ∂b = outer_derivative * a.T
 *
 * @params: a: Node*: pointer to first input Node
 *          b: Node*: pointer to second input Node
*/
class Mul: public OperatorNode<2>{
    using OperatorNode<2>::OperatorNode;

    void setSize() override {
        this->setRows( this->inputs[0]->getRows() );
        this->setCols( this->inputs[1]->getCols() );
    }

    void compute() override {
        this->value =
            this->getInput(0)
            *
            this->getInput(1)
        ;
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override{
        if(input_wrt_index){ //second input
            return 
                this->getInput(0).transpose() 
                * 
                this->outer_derivative;
        }else{ // first input
            return 
                this->outer_derivative
                *
                this->getInput(1).transpose();
        }
    }
};

/**
 * Element wise Multiplication operator: a[i][j] * b[i][j]
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = b
 *      ∂b = a
 *
 * @params: a: Node*: pointer to first input Node
 *          b: Node*: pointer to second input Node
*/
class EleWiseMul: public OperatorNode<2>{
    using OperatorNode<2>::OperatorNode;

    void compute() override{
        this->value =
            this->getInput(0).array()
            *
            this->getInput(1).array()
        ;
    }

    Eigen::MatrixXd derivative(size_t input_wrt_index) override{
        if(input_wrt_index){  //second input
            return 
                this->getInput(0).array() 
                * 
                this->outer_derivative.array()
            ;
        }else{ //first input
            return 
                this->getInput(1).array() 
                * 
                this->outer_derivative.array()
            ;

        }
    }
};

/**
 * Matrix-Scalar multiplication operator: a[i][j] * b
 * TYPE: MATRIX SCALAR
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *
 * Used to multiply a matrix with a scalar value
 *
 * NOTE: The graph will not find the partial derivative with respect to scalar input(second input)
 *
 * @params: a: Node*: pointer to input Node
 *          b: double: scalar value
*/
class ScalarMul: public OperatorNode<2>{
    using OperatorNode<2>::OperatorNode;

    void compute() override {
        double scalar = this->getInput(1)(0, 0);
        this->value = this->getInput() * scalar ;
    }

    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        double scalar = this->getInput(1)(0, 0);
        return scalar * this->outer_derivative.array();
    }
};

/**
 * Element wise Division a[i][j] / b[i][j]
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *      ∂b = 1
 *
 * @params: a: Node*: pointer to first input Node
 *          b: Node*: pointer to second input Node
*/
class EleWiseDiv: public OperatorNode<2>{
    using OperatorNode<2>::OperatorNode;

    void compute() override {
        this->value = 
            this->getInput(0).array()
            /
            this->getInput(1).array()
        ;
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        if(input_wrt_index){ //second input
            return 
                - ( this->getInput(0).array() / 
                    this->getInput(1).array().pow(2) )
                * this->outer_derivative.array()
            ;
        }else{//first input
            return 
                this->getInput(1).array().inverse() 
                *
                this->outer_derivative.array()
            ;
        }
    }
};

/**
 * Matrix-Scalar division operator: a[i][j] / b[0][0]
 * TYPE: MATRIX matrix-SCALAR
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *      ∂b = 1
 *
 * NOTE: the scalar input Node is a 1x1 matrix
 * Use this to get partial derivative with respect to the second input
 *
 * @params: a: Node*: pointer to first input Node
 *          b: Node*: pointer to second input Node (1x1 matrix node)
*/
class Div: public OperatorNode<2>{
    using OperatorNode<2>::OperatorNode;

    void compute() override {
        this->value = 
            this->getInput(0).array()
            /
            this->getInput(1)(0, 0)
        ;
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        if(input_wrt_index){ //second input
            return 
                - ( this->getInput(0).array() / 
                    std::pow(this->getInput(1)(0,0), 2) )
                * this->outer_derivative.array()
            ;
        }else{ //first input
            return 
                (1 / this->getInput(1)(0,0))
                *
                this->outer_derivative.array()
            ;
        }
    }
};

/**
 * Matrix-Scalar division operator: a[i][j] / b
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *
 * Used to multiply a matrix with a scalar value
 *
 * NOTE: The graph will not find the partial derivative with respect to scalar input(second input)
 *
 * @params: a: Node*: pointer to input Node
 *          b: double: scalar value
*/
class ScalarDiv: public OperatorNode<2>{
    using OperatorNode<2>::OperatorNode;
    void compute() override {
        double scalar = this->getInput(1)(0, 0);
        this->value = this->getInput(0) / scalar;
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        double scalar = this->getInput(1)(0, 0);
        return (1 / scalar) * this->outer_derivative;
    }
};

/**
 * Power operator: raise every elements of a matrix to the power of a scalar value. a[i][j]^b
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *
 * @params: a: Node*: pointer to input Node
*/
class Pow: public OperatorNode<2>{
    using OperatorNode<2>::OperatorNode;

    void compute() override{
        double exponent = this->getInput(1)(0,0);
        this->value = this->getInput(0).array().pow(exponent);
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        double exponent = this->getInput(1)(0,0);
        return this->getInput(0).array()
            .pow(exponent - 1) 
            * exponent
            * outer_derivative.array()
        ;
    }
};

/**
 * Square Root operator : √a[i][j]
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *
 * @params: a: Node*: pointer to input Node
*/
class Sqrt:public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override{
        this->value = this->getInput().array().sqrt();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return (
                1 / (2 * this->getInput().array().sqrt())
            ) * this->outer_derivative.array() 
        ;
    }
};

/**
 * Invert or Negation operator : -a[i][j]
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = -1
 *
 * @params: a: Node*: pointer to input Node
*/
class Invert: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = - this->getInput();
    }

    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return -this->outer_derivative;
    }
};

/**
 * Subtraction operator : a[i][j] - b[i][j]
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *      ∂b = -1
 *
 * @params: a: Node*: pointer to first input Node
 *          b: Node*: pointer to second input Node
*/
class Subtract: public OperatorNode<2>{
    using OperatorNode<2>::OperatorNode;

    void compute() override {
        this->value = this->getInput(0) - this->getInput(1);
    }

    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        if(input_wrt_index){ // Second input
            return -this->outer_derivative;
        }else{ // First input
            return this->outer_derivative;
        }
    }
};

/**
 * Inversion operator: 1 / a[i][j]
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = -1
 *
 * @params: a: Node*: pointer to input Node
*/
class Inverse: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().inverse();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return -this->getInput().array().pow(2).inverse()
            *
            this->outer_derivative.array();
    }
};

//==========================================================================
//                              Trigonometric
//==========================================================================

/**
 * Sin operator: sin(a[i][j])
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *
 * @params: a: Node*: pointer to input Node
*/
class Sin: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().sin();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->getInput().array().cos()
            *
            this->outer_derivative.array();
    }
};

/**
 * Cos operator: cos(a[i][j])
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *
 * @params: a: Node*: pointer to input Node
*/
class Cos: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().cos();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return -this->getInput().array().sin()
            *
            this->outer_derivative.array();
    }
};

/**
 * Tan operator: tan(a[i][j])
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *
 * @params: a: Node*: pointer to input Node
*/
class Tan: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().tan();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->getInput().array().cos().pow(2).inverse()
            *
            this->outer_derivative.array();
    }
};

//==========================================================================
//                              Hyperbolic 
//==========================================================================

/**
 * Hyperbolic Sin operator: sinh(a[i][j])
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *
 * @params: a: Node*: pointer to input Node
*/
class Sinh: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().sinh();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->getInput().array().cosh()
            *
            this->outer_derivative.array();
    }
};

/**
 * Hyperbolic Cos operator: cosh(a[i][j])
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *
 * @params: a: Node*: pointer to input Node
*/
class Cosh: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().cosh();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->getInput().array().sinh() 
            *
            this->outer_derivative.array();
    }
};

/**
 * Hyperbolic Cos operator: tanh(a[i][j])
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *
 * @params: a: Node*: pointer to input Node
*/
class Tanh: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().tanh();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->getInput().array().cosh().pow(2).inverse() 
            * 
            this->outer_derivative.array();
    }
};

//==========================================================================
//                              Exp & Log 
//==========================================================================

/**
 * Exponential operator: exp(a[i][j])
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *
 * @params: a: Node*: pointer to input Node
*/
class Exp: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().exp();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->getInput().array().exp() 
            * 
            this->outer_derivative.array();
    }
};

/**
 * Log base e or Ln operator: ln(a[i][j])
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *
 * @params: a: Node*: pointer to input Node
*/
class Loge: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override {
        this->value = this->getInput().array().log();
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->getInput().array().inverse() 
            * 
            this->outer_derivative.array();
    }
};

//==========================================================================
//                              Matrix Function
//==========================================================================

/**
 * Sum operator: sum all of the element in a matrix into a 1x1 matrix: Σa[i][j]
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *
 * @params: a: Node*: pointer to input Node
*/
class Sum: public OperatorNode<1>{
    public:
        Sum(std::initializer_list<Node*> input_list)
        :OperatorNode<1>(input_list){
            this->rows = this->getInput().rows();
            this->cols = this->getInput().cols();
        }

        Sum(Node* input)
        :OperatorNode<1>(input){
            this->rows = this->getInput().rows();
            this->cols = this->getInput().cols();
        }

    void compute() override {
        this->value = 
            Eigen::MatrixXd::Constant(
                1, 1, this->getInput().sum()
            )
        ;
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return this->outer_derivative;
    }
};

//==========================================================================
//                              Statistic Function
//==========================================================================

/**
 * Rectifier Linear Unit operator: max(0, a[i][j])
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *
 * @params: a: Node*: pointer to input Node
*/
class ReLU: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override{
        this->value = 
            this->getInput().cwiseMax(0)
        ;
    }

    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        return (this->getInput().array() > 0).cast<double>().array() 
            * 
            this->outer_derivative.array()
        ;
    }
};

/**
 * Leaky Rectifier Linear Unit operator: max(leak_value, a[i][j])
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 1
 *
 * NOTE: default leak_value = 0.1
 *
 * @params: a: Node*: pointer to input Node
*/
class LeakyReLU: public OperatorNode<2>{
    using OperatorNode<2>::OperatorNode;

    void compute() override{
        double leak_value = this->getInput(1)(0, 0);
        if(leak_value == 0) leak_value = 0.1;

        this->value =
            this->getInput().cwiseMax(leak_value * this->getInput())
        ;
    }
    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        double leak_value = this->getInput(1)(0, 0);
        if(leak_value == 0) leak_value = 0.1;

        size_t rows = this->getInput().rows();
        size_t cols = this->getInput().cols();

        Eigen::MatrixXd one_m = Eigen::MatrixXd::Constant(rows, cols, 1);
        Eigen::MatrixXd leak_m = Eigen::MatrixXd::Constant(rows, cols, leak_value);

        return (this->getInput().array() > 0).select(one_m, leak_m).array()
            *
            this->outer_derivative.array()
        ;
    }
};

/**
 * Sigmoid operator: 1 / (1 + e^(-a[i][j]))
 * TYPE: ELEMENT WISE
 *
 * DERIVATIVE: 
 *      ∂a = 
 *
 * @params: a: Node*: pointer to input Node
*/
class Sigmoid: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override{
        //1 / (1 + exp(-x))
        this->value = 
            (
                1+(-this->getInput().array())
                .exp()
            ).inverse()
        ;
    }

    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        Eigen::MatrixXd temp = (-this->getInput().array()).exp();
        return temp.array() * this->outer_derivative.array() 
            / 
            ( 1 + temp.array() ).pow(2);
    }
};

/**
 * Softmax operator: e^a[i][j] / Σe^a (e^a is element wise)
 * TYPE: ELEMENT WISE + NORMALIZATION
 *
 * DERIVATIVE: 
 *      exp = e^a (element wise)
 *      sum = Σexp
 *      diagonal_exp_sum = diagonal(exp * sum) 
 *
 *          [ exp11 . sum         0              0      ] <- diagonal_exp_sum
 *          [       0        exp22 . sum         0      ]
 *          [       0             0         exp33 . sum ]
 *
 *      jacobian = ( diagonal_exp_sum - square_transpose_elewise_mul ) / sum^2
 *
 *          [ exp11sum - e1e1           e1e2                 e1e3     ] <-jacobian
 *          [       e2e1          exp22sum - e2e2            e2e3     ]
 *          [       e3e1                e3e2          exp33sum - e3e3 ]
 *
 *      ∂a = jacobian * outer derivative
 *
 * @params: a: Node*: pointer to input Node
*/
class Softmax: public OperatorNode<1>{
    using OperatorNode<1>::OperatorNode;

    void compute() override{
        Eigen::MatrixXd exp = this->getInput().array().exp(); 
        double sum = exp.sum();

        this->value = exp / sum;
    }

    Eigen::MatrixXd derivative(size_t input_wrt_index) override {
        //(Diagonal(x * sum) - OuterProd(X, X)) / sum^2
        Eigen::MatrixXd exp = this->getInput().array().exp();
        double sum = exp.sum();

        Eigen::MatrixXd diagonal_exp_sum = Eigen
            ::MatrixXd
            ::Constant(exp.rows(), exp.rows(), 0);
        diagonal_exp_sum.diagonal() = exp * sum;

        Eigen::MatrixXd jacobian = 
            ( diagonal_exp_sum - (exp * exp.transpose()) ) 
            / 
            std::pow(sum, 2)
        ;
        return jacobian * this->outer_derivative;
    }
};

}//namespace nodeflow ----------------------------------------
