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
    error::report("Graph::define (subgraph)", "parameter not declared",
                  subgraph_name, 0);
  subgraph_nodes_map[subgraph_name] = subgraph.getRoot();
  return *this;
}

string Graph::getOperatorName(Token op_token) {
  if (ops_symbol_map.find(op_token.type) == ops_symbol_map.end())
    error::report("Graph::getOperatorName", "Undefined Symbol", op_token.value,
                  0);
  return ops_symbol_map[op_token.type];
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
