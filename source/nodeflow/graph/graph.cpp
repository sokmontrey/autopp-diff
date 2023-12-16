#include "./graph.hpp"

using namespace nodeflow;
using namespace std;

Graph &Graph::finished() {
  if (root == nullptr)
    error::report("Graph::finished", "root is not defined", "", 0);
  root->finished();
  return *this;
}

Node *Graph::getRoot() { return root; }

Graph &Graph::evaluate(bool is_reset) {
  if (root == nullptr)
    error::report("Graph::evaluate", "root is not defined", "", 0);
  if (is_reset)
    root->reset();
  root->forward();
  return *this;
}

Graph &Graph::gradient(bool is_reset) {
  if (root == nullptr)
    error::report("Graph::backward", "root is not defined", "", 0);
  if (is_reset)
    root->reset();
  root->backward();
  return *this;
}

Graph &Graph::reset() {
  if (root == nullptr)
    error::report("Graph::reset", "root is not defined", "", 0);
  root->reset();
  return *this;
}

Graph &Graph::print() {
  if (root == nullptr)
    error::report("Graph::print", "root is not defined", "", 0);
  root->print();
  return *this;
}

Eigen::MatrixXd Graph::partial(string node_name) {
  if (root == nullptr)
    error::report("Graph::partial", "root is not defined", "", 0);
  if (nodes_map.find(node_name) == nodes_map.end())
    error::report("Graph::partial", "node not defined", node_name, 0);
  return nodes_map[node_name].getGrad();
}

//=============================================================================
//                                  Getters
//=============================================================================

Node *Graph::node(string node_name) {
  if (nodes_map.find(node_name) == nodes_map.end())
    error::report("Graph::node", "node not defined", node_name, 0);
  return &nodes_map[node_name];
}

double Graph::value(string node_name, int row, int col) {
  if (nodes_map.find(node_name) == nodes_map.end())
    error::report("Graph::value", "node not defined", node_name, 0);
  return nodes_map[node_name].getValue(row, col);
}

Eigen::MatrixXd &Graph::matrix(string node_name) {
  if (nodes_map.find(node_name) == nodes_map.end())
    error::report("Graph::matrix", "node not defined", node_name, 0);
  return nodes_map[node_name].getMatrix();
}

//=============================================================================
//                                  Setters
//=============================================================================

//=============================================================================
//                              Private Methods
//=============================================================================

string Graph::getOperatorName(Token op_token) {
  if (ops_symbol_map.find(op_token.type) == ops_symbol_map.end())
    error::report("Graph::getOperatorName", "Undefined Symbol", op_token.value,
                  0);
  return ops_symbol_map[op_token.type];
}

void Graph::parse() {
  ExScanner scanner(expression);
  ExParser parser(scanner.scan());
  buildGraph(parser.parse());
  finished();
}

void Graph::buildGraph(ExNode *ex_root) {
  deque<Node *> operators;

  ex_root->reverse_iterate([&](ExNode *ex_node) {
    string value = ex_node->token.value;

    switch (ex_node->type) {
    case ExType::NAME: {
      if (nodes_map.find(value) == nodes_map.end())
        error::report("Graph::buildGraph NAME", "parameter not defined", value,
                      0);
      operators.push_back(&nodes_map[value]);
    } break;
    case ExType::NUMBER: {
      if (nodes_map.find(value) == nodes_map.end())
        nodes_map.insert({value, Node::Scalar(stod(value))});
      operators.push_back(&nodes_map[value]);
    } break;
    case ExType::CONSTANT: {
      operators.back()->constant();
      break;
    }
    case ExType::SUBGRAPH: {
      if (subgraph_nodes_map.find(value) == subgraph_nodes_map.end())
        error::report("Graph::buildGraph SUBGRAPH", "subgraph not declared",
                      value, 0);
      if (subgraph_nodes_map[value] == nullptr)
        error::report("Graph::buildGraph SUBGRAPH", "subgraph not defined",
                      value, 0);
      operators.push_back(subgraph_nodes_map[value]);
    } break;
    case ExType::SYMBOL:
    case ExType::FUNCTION: {
      string name = value;
      if (ex_node->type == ExType::SYMBOL)
        name = getOperatorName(ex_node->token);
      if (one_arg_ops_map.find(name) != one_arg_ops_map.end()) {
        Node *a = operators.back();
        operators.pop_back();
        operators.push_back(one_arg_ops_map[name](a));
        break;
      }
      if (two_args_ops_map.find(name) != two_args_ops_map.end()) {
        Node *b = operators.back();
        operators.pop_back();
        Node *a = operators.back();
        operators.pop_back();
        operators.push_back(two_args_ops_map[name](a, b));
        break;
      }
    } break;
    }
  });

  root = operators.back();
}
