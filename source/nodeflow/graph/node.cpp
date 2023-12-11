#include "./node.hpp"

using namespace nodeflow;
/**
 * A Node class that is used to build a graph.
 *
 * @constructors:
 *      @params: Eigen::Matrix or Eigen::Vector : Assign an Eigen::Matrix or
 * Vector object to the value member.
 */
void Node::setRows(size_t rows) { this->rows = rows; }
void Node::setCols(size_t cols) { this->cols = cols; }

Node::Node(Eigen::MatrixXd initial_value) {
  this->value = initial_value;

  this->setRows(initial_value.rows());
  this->setCols(initial_value.cols());
}

//================================================================
//                          Getters
//================================================================

size_t Node::getRows() { return this->rows; }
size_t Node::getCols() { return this->cols; }
Eigen::MatrixXd &Node::operator()() { return this->value; }
double &Node::operator()(size_t row, size_t col) {
  return this->value(row, col);
}
double Node::getValue(size_t row, size_t col) { return this->value(row, col); }
double Node::getValue() { return this->value(0, 0); }
Eigen::MatrixXd &Node::getMatrix() { return this->value; }
Eigen::MatrixXd &Node::getGrad() {
  if (!this->is_differentiatable)
    std::cout << "Warning: Calling 'getGrad()' on a constant node" << std::endl;
  return this->outer_derivative;
}
std::string Node::getName() { return this->name; }
void Node::print() {
  std::cout << "Node: "
            << " (Matrix) "
            << "\n"
            << "----\n"
            << this->value << "\n"
            << "----\n";
}

//================================================================
//                          Setters
//================================================================

void Node::setMatrix(Eigen::MatrixXd new_value) {
  this->value = new_value;

  this->setRows(new_value.rows());
  this->setCols(new_value.cols());
}
void Node::setValue(size_t row, size_t col, double new_value) {
  this->value(row, col) = new_value;
}
void Node::setValue(double new_value) { this->value(0, 0) = new_value; }
void Node::setName(std::string new_name) { this->name = new_name; }
void Node::operator=(Eigen::MatrixXd new_value) {
  this->value = new_value;

  this->setRows(new_value.rows());
  this->setCols(new_value.cols());
}

//================================================================
//                       Node Methods
//================================================================

bool Node::isDifferentiatable() { return this->is_differentiatable; }
bool Node::isConstant() { return this->is_constant; }
Node &Node::constant() {
  this->is_constant = true;
  this->is_differentiatable = false;
  return *this;
}

//================================================================
//                       Graph Methods
//================================================================

std::vector<Node *> Node::getAllLeaveNode() {
  return std::vector<Node *>{this};
}

void Node::finished() { this->finished(true); }
void Node::finished(bool is_child) {
  this->setRows(this->value.rows());
  this->setCols(this->value.cols());
  this->num_parent++;
}
void Node::clearGraph() {
  this->num_parent = 0;
  return;
}
void Node::reset() {
  this->is_value_ready = false;
  this->parent_called_count = 0;
}
Eigen::MatrixXd &Node::forward() { return this->value; }
void Node::backward(Eigen::MatrixXd partial_outer_derivative) {
  if (!this->is_differentiatable)
    return;

  if (!this->parent_called_count) {
    this->outer_derivative = partial_outer_derivative;
  } else {
    this->outer_derivative += partial_outer_derivative;
  }

  this->parent_called_count++;
}
void Node::backward() { /*a method with no pupose just for the graph*/
}

//================================================================
//                        Scalar Methods
//================================================================

Node Node::Random() { return Node(Eigen::MatrixXd::Random(1, 1)); }
Node Node::Scalar(double initial_value) {
  return Node(Eigen::MatrixXd::Constant(1, 1, initial_value));
}
Node Node::ScalarConst(double initial_value) {
  return Node::Scalar(initial_value).constant();
}

//================================================================
//                        Vector Methods
//================================================================

Node Node::Random(size_t rows) {
  return Node(Eigen::MatrixXd::Random(rows, 1));
}
Node Node::Vector(size_t rows) {
  return Node(Eigen::MatrixXd::Constant(rows, 1, 0));
}
Node Node::Vector(size_t rows, double fill_value) {
  return Node(Eigen::MatrixXd::Constant(rows, 1, fill_value));
}
Node Node::Vector(std::initializer_list<double> initial_vector) {
  Node temp = Eigen::MatrixXd(initial_vector.size(), 1);
  for (int i = 0; i < initial_vector.size(); i++) {
    temp.setValue(i, 0, *(initial_vector.begin() + i));
  }
  return temp;
}

//================================================================
//                        Matrix Methods
//================================================================

Node Node::Random(size_t rows, size_t cols) {
  return Node(Eigen::MatrixXd::Random(rows, cols));
}
Node Node::Matrix(size_t rows, size_t cols) {
  return Node(Eigen::MatrixXd::Constant(rows, cols, 0));
}
Node Node::Matrix(size_t rows, size_t cols, size_t fill_value) {
  return Node(Eigen::MatrixXd::Constant(rows, cols, fill_value));
}
Node Node::Matrix(
    std::initializer_list<std::initializer_list<double>> initial_matrix) {
  Node temp = Eigen::MatrixXd(initial_matrix);
  return temp;
}
/**
 * Based class for Operator Nodes.
 *
 * @methods:
 * .finished() must be used at the final OperatorNode to finish setting up the
 * graph .forward() is for forward evaluation of the function Any parameters for
 * the function has to be set directly to the input Node This can be done using
 * node_name = Eigen::Matrix OR Eigen::Vector OR node_name.getValue(row, col) =
 * new_element_value OR node_name(row, col) = new_element_value .backward()
 * propagate backward to calculate partial derivative of the function with
 * respect to each input Nodes Can be called independanly from .forward(). Does
 * not need to call .foward() to operate .backward() .reset() must be used after
 * each .forward() or .backward() call
 * @constructors:
 *      1. @params: input_list: initializer_list of Node* to input Node(s)
 *      2. @params: a: pointer to a single input Node
 *      3. @params: a, b: pointer to first and second input Node
 */
void OperatorNode::setSize() {
  this->setRows(this->inputs[0]->getRows());
  this->setCols(this->inputs[0]->getCols());
}

OperatorNode::OperatorNode(std::initializer_list<Node *> input_list) {
  this->initializeInput(input_list);
}
OperatorNode::OperatorNode(Node *a) {
  this->inputs.push_back(a);
  this->num_inputs = 1;
}
OperatorNode::OperatorNode(Node *a, Node *b) {
  this->inputs.push_back(a);
  this->inputs.push_back(b);
  this->num_inputs = 2;
}
void OperatorNode::initializeInput(std::initializer_list<Node *> input_list) {
  for (size_t i = 0; i < this->num_inputs; i++) {
    auto input = *(input_list.begin() + i);
    this->inputs.push_back(input);
  }

  this->setSize();
  this->num_inputs = input_list.size();
}
void OperatorNode::addInput(Node *new_input) {
  this->inputs.push_back(new_input);
  this->num_inputs++;
}

//================================================================
//                          Getters
//================================================================

Eigen::MatrixXd OperatorNode::getInput(size_t input_index) {
  return this->inputs[input_index]->getMatrix();
}
Eigen::MatrixXd OperatorNode::getInput() {
  return this->inputs[0]->getMatrix();
}

//================================================================
//                      Graph Methods
//================================================================

std::vector<Node *> OperatorNode::getAllLeaveNode() {
  std::vector<Node *> nodes;
  for (size_t i = 0; i < this->num_inputs; i++) {
    auto temp = this->inputs[i]->getAllLeaveNode();
    nodes.insert(nodes.end(), temp.begin(), temp.end());
  }
  return nodes;
}
void OperatorNode::clearGraph() {
  this->num_parent = 0;
  for (size_t i = 0; i < this->num_inputs; i++) {
    this->inputs[i]->clearGraph();
  }
  return;
}
void OperatorNode::reset() {
  if (!this->is_value_ready)
    return;

  this->is_value_ready = false;
  this->parent_called_count = 0;

  for (size_t i = 0; i < this->num_inputs; i++) {
    this->inputs[i]->reset();
  }
}
void OperatorNode::finished() {
  clearGraph();
  finished(true);
}
void OperatorNode::finished(bool is_child) {
  this->num_parent++;

  bool is_diff_temp = false;
  for (size_t i = 0; i < this->num_inputs; i++) {
    this->inputs[i]->finished();
    // if all children are constant, then parent is also constant
    is_diff_temp = this->inputs[i]->isDifferentiatable() || is_diff_temp;
  }
  this->is_differentiatable = is_diff_temp;

  this->setSize();
}
Eigen::MatrixXd &OperatorNode::forward() {
  if (this->is_value_ready)
    return this->value;

  for (size_t i = 0; i < this->num_inputs; i++) {
    this->inputs[i]->forward();
  }
  this->compute();

  this->is_value_ready = true;
  return this->value;
}
void OperatorNode::backward() {
  this->backward(Eigen::MatrixXd::Constant(this->rows, this->cols, 1.0));
}
void OperatorNode::backward(Eigen::MatrixXd partial_outer_derivative) {
  if (!this->is_differentiatable)
    return;

  if (!this->parent_called_count) {
    this->outer_derivative = partial_outer_derivative;
  } else {
    this->outer_derivative += partial_outer_derivative;
  }

  this->parent_called_count++;

  if (this->parent_called_count >= this->num_parent) {
    for (size_t i = 0; i < this->num_inputs; i++) {
      Eigen::MatrixXd partial_derivative = this->derivative(i);
      this->inputs[i]->backward(partial_derivative);
    }
  }
}
