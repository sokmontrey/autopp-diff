#include "./graph.hpp"

using namespace nodeflow;
using namespace std;

Graph &Graph::declare(string param) {
  if (nodes_map.find(param) != nodes_map.end())
    error::report("Graph::declare", "parameter already declared", param, 0);

  if (param[0] == '$')
    subgraph_nodes_map.insert({param.substr(1), nullptr});
  else if (param[1] != '\0' && param[1] == '$')
    subgraph_nodes_map.insert({param.substr(2), nullptr});
  else
    nodes_map.insert({param, Node::Scalar(0)});

  return *this;
}

// for node
Graph &Graph::define(string node_name, Node node) {
  if (nodes_map.find(node_name) == nodes_map.end())
    error::report("Graph::define", "parameter not declared", node_name, 0);
  nodes_map[node_name] = node;
  return *this;
}

// for subgraph
Graph &Graph::define(string subgraph_name, Graph &subgraph) {
  if (subgraph_nodes_map.find(subgraph_name) == subgraph_nodes_map.end())
    error::report("Graph::define", "parameter not declared", subgraph_name, 0);
  subgraph_nodes_map[subgraph_name] = subgraph.getRoot();
  return *this;
}

Node *Graph::createOperator(string op_name, Node *a) {
  if (one_args_ops_map.find(op_name) == one_args_ops_map.end())
    return nullptr;
  return one_args_ops_map[op_name](a);
}
Node *Graph::createOperator(string op_name, Node *a, Node *b) {
  if (two_args_ops_map.find(op_name) == two_args_ops_map.end())
    return nullptr;
  return two_args_ops_map[op_name](a, b);
}

string Graph::getOperatorName(Token op_token) {
  if (ops_symbol_map.find(op_token.type) == ops_symbol_map.end())
    error::report("Graph::getOperatorName", "Undefined Symbol", op_token.value,
                  0);
  return ops_symbol_map[op_token.type];
}

Node *Graph::createFunction(string func_name, Node *a, Node *b) {
  Node *func = nullptr;

  if (a == nullptr) {
    error::report("Graph::createFunction", "Undefined Function",
                  func_name + " with NO argument", 0);
  }

  if (b == nullptr) {
    func = createOperator(func_name, a);
    if (func == nullptr)
      error::report("Graph::createFunction", "Undefined Function",
                    func_name + " with ONE argument", 0);
  } else {
    func = createOperator(func_name, a, b);
    if (func == nullptr)
      error::report("Graph::createFunction", "Undefined Function",
                    func_name + " with TWO arguments", 0);
  }

  return func;
}

void Graph::operator=(const char *expression) { parse(expression); }

void Graph::parse(string expression) {
  this->expression = expression;

  ExScanner scanner(expression);
  ExParser parser(scanner.scan());
  buildGraph(parser.parse());
  root->finished();
}

void Graph::buildGraph(ExNode *ex_root) {
  if (root != nullptr)
    error::report("Graph::buildGraph", "expression is already defined",
                  expression, 0);

  Node *nodes[2] = {nullptr, nullptr};

  ex_root->reverse_iterate([&](ExNode *ex_node) {
    string value = ex_node->token.value;

    switch (ex_node->type) {
    case ExType::NAME: {
      if (nodes_map.find(value) == nodes_map.end())
        error::report("Graph::buildGraph NAME", "parameter not defined", value,
                      0);
      nodes[nodes[0] == nullptr ? 0 : 1] = &nodes_map[value];
    } break;
    case ExType::NUMBER: {
      if (nodes_map.find(value) == nodes_map.end())
        nodes_map.insert({value, Node::Scalar(stod(value))});
      nodes[nodes[0] == nullptr ? 0 : 1] = &nodes_map[value];
    } break;
    case ExType::CONSTANT: {
      nodes[nodes[1] == nullptr ? 0 : 1]->constant();
      break;
    }
    case ExType::SUBGRAPH: {
      if (subgraph_nodes_map.find(value) == subgraph_nodes_map.end())
        error::report("Graph::buildGraph SUBGRAPH", "subgraph not declared",
                      value, 0);
      if (subgraph_nodes_map[value] == nullptr)
        error::report("Graph::buildGraph SUBGRAPH", "subgraph not defined",
                      value, 0);
      nodes[nodes[0] == nullptr ? 0 : 1] = subgraph_nodes_map[value];
    } break;
    case ExType::FUNCTION: {
      nodes[0] = createFunction(value, nodes[0], nodes[1]);
      nodes[1] = nullptr;
    } break;
    case ExType::SYMBOL: {
      string op_name = getOperatorName(ex_node->token);
      nodes[0] = createOperator(op_name, nodes[0], nodes[1]);
      nodes[1] = nullptr;
    } break;
    }
  });

  if (nodes[1] != nullptr)
    error::report("Graph::buildGraph",
                  "invalid expression (expression should end with a function)",
                  "", 0);

  this->root = nodes[0];
}

Graph &Graph::operator()() { return evaluate(); }
Graph &Graph::evaluate() {
  root->forward();
  return *this;
}

Graph &Graph::gradient() {
  root->backward();
  return *this;
}

Eigen::MatrixXd Graph::partial(string node_name) {
  if (nodes_map.find(node_name) == nodes_map.end())
    error::report("Graph::partial", "parameter not declared", node_name, 0);
  return nodes_map[node_name].getGrad();
}

Node *Graph::getRoot() { return root; }

Graph &Graph::print() {
  root->print();
  return *this;
}
