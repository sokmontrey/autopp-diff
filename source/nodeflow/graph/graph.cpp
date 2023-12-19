#include "./graph.hpp"

using namespace nodeflow;
using namespace std;

void Graph::destroy() {
  if (this->root) {
    this->root->reverse_iterate([](Node *node) {
      if (node != nullptr && node->getType() != NodeType::NODE)
        delete node;
    });
  }
}

void Graph::subgraph() {
  if (root == nullptr)
    error::report("Graph::subgraph", "root is not defined", "", 0);
  is_subgraph = true;
}

Graph &Graph::finished() {
  if (root == nullptr)
    error::report("Graph::finished", "root is not defined", "", 0);
  root->finished();
  return *this;
}

Node *Graph::getRoot() { return root; }

const Eigen::MatrixXd &Graph::evaluate(bool is_reset) {
  if (root == nullptr)
    error::report("Graph::evaluate", "root is not defined", "", 0);
  if (is_reset)
    root->reset();
  root->forward();
  return root->getMatrix();
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

const Eigen::MatrixXd &Graph::partial(string node_name) {
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
  ExNode *ex_root = parser.parse();
  buildGraph(ex_root);
  finished();
  ex_root->reverse_iterate([](ExNode *ex_node) { delete ex_node; });
}

void Graph::buildGraph(ExNode *ex_root) {
  deque<Node *> nodes;

  ex_root->reverse_iterate([&](ExNode *ex_node) {
    string value = ex_node->token.value;

    switch (ex_node->type) {
    case ExType::NAME: {
      if (nodes_map.find(value) == nodes_map.end())
        error::report("Graph::buildGraph NAME", "parameter not defined", value,
                      0);
      nodes.push_back(&nodes_map[value]);
    } break;
    case ExType::NUMBER: {
      if (nodes_map.find(value) == nodes_map.end())
        nodes_map.insert({value, Node::Scalar(stod(value))});
      nodes.push_back(&nodes_map[value]);
    } break;
    case ExType::CONSTANT: {
      nodes.back()->constant();
      break;
    }
    case ExType::SUBGRAPH: {
      if (subgraph_nodes_map.find(value) == subgraph_nodes_map.end())
        error::report("Graph::buildGraph SUBGRAPH", "subgraph not declared",
                      value, 0);
      if (subgraph_nodes_map[value] == nullptr)
        error::report("Graph::buildGraph SUBGRAPH", "subgraph not defined",
                      value, 0);
      nodes.push_back(subgraph_nodes_map[value]);
    } break;
    case ExType::SYMBOL:
    case ExType::FUNCTION: {
      string name = value;
      if (ex_node->type == ExType::SYMBOL)
        name = getOperatorName(ex_node->token);

      Node *op = nullptr;
      if (one_arg_ops_map.find(name) != one_arg_ops_map.end()) {
        Node *a = nodes.back();
        nodes.pop_back();
        op = one_arg_ops_map[name](a);
      }

      if (two_args_ops_map.find(name) != two_args_ops_map.end()) {
        Node *b = nodes.back();
        nodes.pop_back();
        Node *a = nodes.back();
        nodes.pop_back();
        op = two_args_ops_map[name](a, b);
      }
      nodes.push_back(op);
    } break;
    }
  });

  root = nodes.back();
}
