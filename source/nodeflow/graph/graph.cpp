#include "graph.hpp"

using namespace nodeflow;

//                         Initializer
//================================================================

void Graph::init() {
  this->operator_nodes.clear();
  // this->f = parse(removeSpaces(this->expression));
  // this->f->finished();
  if (this->expression == "")
    error::report("Graph", "Expression is empty!", this->expression, 0);
  ExScanner scanner(this->expression);
  scanner.scan();
  scanner.printTokens();
  ExParser parser(scanner.getTokens());
  std::cout << "\nExpression: ";
  parser.expression()->print();
}

//================================================================
//                  No sub_graphs constructor
//================================================================

Graph::Graph(std::string s) {
  this->expression = s;
  init();
}
Graph::Graph(std::string s, std::map<std::string, Node> node_map) {
  this->expression = s;
  this->node_map = node_map;
  init();
}

//================================================================
//                          No node_map
//================================================================

// pointer to subgraphs (end node)
Graph::Graph(std::string s, std::map<std::string, Node *> subgraphs) {
  this->expression = s;
  this->sub_graphs = subgraphs;
  init();
}
// pointer to subgraphs
Graph::Graph(std::string s, std::map<std::string, Graph *> subgraphs) {
  this->expression = s;
  for (auto it = subgraphs.begin(); it != subgraphs.end(); it++) {
    this->sub_graphs[it->first] = it->second->getEnd();
  }
  init();
}
// reference to subgraphs
Graph::Graph(std::string s, std::map<std::string, Graph &> subgraphs) {
  this->expression = s;
  for (auto it = subgraphs.begin(); it != subgraphs.end(); it++) {
    this->sub_graphs[it->first] = it->second.getEnd();
  }
  init();
}

//================================================================
//                  node_map with subgraphs
//================================================================

// pointer to subgraphs (end node)
Graph::Graph(std::string s, std::map<std::string, Node> node_map,
             std::map<std::string, Node *> subgraphs) {
  this->expression = s;
  this->node_map = node_map;
  this->sub_graphs = subgraphs;
  init();
}
// pointer to subgraphs
Graph::Graph(std::string s, std::map<std::string, Node> node_map,
             std::map<std::string, Graph *> subgraphs) {
  this->expression = s;
  this->node_map = node_map;
  for (auto it = subgraphs.begin(); it != subgraphs.end(); it++) {
    this->sub_graphs[it->first] = it->second->getEnd();
  }
  init();
}
// reference to subgraphs
Graph::Graph(std::string s, std::map<std::string, Node> node_map,
             std::map<std::string, Graph &> subgraphs) {
  this->expression = s;
  this->node_map = node_map;
  for (auto it = subgraphs.begin(); it != subgraphs.end(); it++) {
    this->sub_graphs[it->first] = it->second.getEnd();
  }
  init();
}
Graph::~Graph() {
  for (int i = 0; i < operator_nodes.size(); i++) {
    delete this->operator_nodes[i];
  }
}

//================================================================
//                        Node Methods
//================================================================

Graph &Graph::constant(std::string name) {
  this->node_map[name].constant();
  return *this;
}
Graph &Graph::setNode(std::string name, Node node) {
  this->node_map[name] = node;
  return *this;
}
Node *Graph::getNode(std::string name) { return &this->node_map[name]; }

std::vector<std::string> Graph::getVariableName() {
  std::vector<std::string> variable_names;
  for (auto it = this->node_map.begin(); it != this->node_map.end(); it++) {
    if (!it->second.isConstant()) {
      variable_names.push_back(it->first);
    }
  }
  return variable_names;
}

std::vector<Node *> Graph::getVariableNode() {
  std::vector<Node *> variable_nodes;
  for (auto it = this->node_map.begin(); it != this->node_map.end(); it++) {
    if (!it->second.isConstant()) {
      variable_nodes.push_back(&it->second);
    }
  }
  return variable_nodes;
}

std::vector<Node *> Graph::getAllLeaveNode() {
  return this->f->getAllLeaveNode();
}

std::vector<Node *> Graph::getAllVariableNode() {
  std::vector<Node *> all_nodes = this->getAllLeaveNode();
  for (int i = 0; i < all_nodes.size(); i++) {
    if (all_nodes[i]->isConstant()) {
      all_nodes.erase(all_nodes.begin() + i);
      i--;
    }
  }
  return all_nodes;
}

Node *Graph::getOperatorNode(int i) { return this->operator_nodes[i]; }
size_t Graph::getOperatorLength() { return this->operator_nodes.size(); }

//================================================================
//                        Eigen Methods
//================================================================
/**
 * Methods for directly getting and setting Eigen::MatrixXd
 */

/* getter */
Eigen::MatrixXd Graph::getGrad(std::string name) {
  return this->node_map[name].getGrad();
}

double Graph::getValue(std::string name, size_t i, size_t j) {
  return this->node_map[name].getValue(i, j);
}
double Graph::getValue(std::string name) {
  return this->node_map[name].getValue(0, 0);
}
double Graph::getValue() { return this->f->getValue(0, 0); }

Eigen::MatrixXd Graph::getMatrix(std::string name) {
  return this->node_map[name].getMatrix();
}
Eigen::MatrixXd Graph::getMatrix() { return this->f->getMatrix(); }

/* end node getter */
Node *Graph::getF() { return this->f; }
Node *Graph::getEnd() { return this->f; }

/* setter */
Graph &Graph::setValue(std::string name, size_t i, size_t j, double value) {
  this->node_map[name].setValue(i, j, value);
  return *this;
}
Graph &Graph::setValue(std::string name, double value) {
  this->node_map[name].setValue(0, 0, value);
  return *this;
}
Graph &Graph::setMatrix(std::string name, Eigen::MatrixXd value) {
  this->node_map[name].setMatrix(value);
  return *this;
}

//================================================================
//                      Function Methods
//================================================================
/**
 * Methods that are used on the last operator (f)
 *   without having to get f from the graph
 */
Graph &Graph::finished() {
  this->f->finished();
  return *this;
}
Graph &Graph::forward() {
  this->f->reset();
  this->f->forward();
  return *this;
}
Eigen::MatrixXd Graph::operator()() { return this->f->forward(); }
Graph &Graph::backward() {
  this->f->reset();
  this->f->backward();
  return *this;
}
Graph &Graph::backward(Eigen::MatrixXd partial_outer_derivative) {
  this->f->backward(partial_outer_derivative);
  return *this;
}
Graph &Graph::print() {
  this->f->print();
  return *this;
}
Graph &Graph::reset() {
  this->f->reset();
  return *this;
}

/*
Node *Graph::createOperator(Node *a, Node *b, std::string operator_name) {
  if (operator_name[0] == '#') {
    Node *n = Graph::createOperator(a, b, operator_name.substr(1));
    n->constant();
    return n;
  }

  //================================================================
  //                           Basic
  //================================================================
  if (operator_name == "add")
    return new Add(a, b);
  else if (operator_name == "sub" || operator_name == "subtract")
    return new Subtract(a, b);
  else if (operator_name == "mul" || operator_name == "matmul" ||
           operator_name == "dot")
    return new Mul(a, b);
  else if (operator_name == "ew_mul")
    return new EleWiseMul(a, b);
  else if (operator_name == "s_mul")
    return new ScalarMul(a, b);
  else if (operator_name == "div")
    return new Div(a, b);
  else if (operator_name == "ew_div")
    return new EleWiseDiv(a, b);
  else if (operator_name == "s_div")
    return new ScalarDiv(a, b);
  else if (operator_name == "pow")
    return new Pow(a, b);
  else if (operator_name == "sqrt")
    return new Sqrt(a);
  else if (operator_name == "invert")
    return new Invert(a);
  else if (operator_name == "inverse")
    return new Inverse(a);
  //================================================================
  //                        Trigonometry
  //================================================================
  else if (operator_name == "sin")
    return new Sin(a);
  else if (operator_name == "cos")
    return new Cos(a);
  else if (operator_name == "tan")
    return new Tan(a);
  //================================================================
  //                        Hyperbolic
  //================================================================
  else if (operator_name == "sinh")
    return new Sinh(a);
  else if (operator_name == "cosh")
    return new Cosh(a);
  else if (operator_name == "tanh")
    return new Tanh(a);
  //================================================================
  //                        Exponential
  //================================================================
  else if (operator_name == "exp")
    return new Exp(a);
  //================================================================
  //                        Logarithmic
  //================================================================
  else if (operator_name == "ln")
    return new Loge(a);
  //================================================================
  //                      Matrix Operators
  //================================================================
  else if (operator_name == "sum")
    return new Sum(a);
  //================================================================
  //                          Statistic
  //================================================================
  else if (operator_name == "sigmoid")
    return new Sigmoid(a);
  else if (operator_name == "relu")
    return new ReLU(a);
  else if (operator_name == "leaky_relu")
    return new LeakyReLU(a, b);
  else if (operator_name == "softmax")
    return new Softmax(a);
  else
    throw std::invalid_argument("Operator " + operator_name + " is not found");
}
*/
