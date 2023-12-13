#include "./graph.hpp"

using namespace nodeflow;
using namespace std;

Graph &Graph::declare(string param) {
  if (nodes_map.find(param) != nodes_map.end())
    error::report("Graph", "parameter already declared", param, 0);

  if (param[0] == '$' || (param[1] != '\0' && param[1] == '$'))
    subgraphs_map.insert({param, nullptr});
  else
    nodes_map.insert({param, Node::Scalar(0)});

  return *this;
}

// for node
Graph &Graph::define(string node_name, Node node) {
  if (nodes_map.find(node_name) == nodes_map.end())
    error::report("Graph", "parameter not declared", node_name, 0);
  nodes_map[node_name] = node;
  return *this;
}
// for subgraph
Graph &Graph::define(string node_name, Node *node) {
  if (subgraphs_map.find(node_name) == subgraphs_map.end())
    error::report("Graph", "parameter not declared", node_name, 0);
  subgraphs_map[node_name] = node;
  return *this;
}

Node *Graph::createOperator(string op_name, Node *a, Node *b) {
  if (ops_name_map.find(op_name) == ops_name_map.end())
    error::report("Graph", "Undefined Operator", op_name, 0);
  // TODO: check if args[0] and args[1] is a thing
  return ops_name_map[op_name](a, b);
}

string Graph::getOperatorName(Token op_token) {
  if (ops_symbol_map.find(op_token.type) == ops_symbol_map.end())
    error::report("Graph", "Undefined Symbol", op_token.value, 0);
  return ops_symbol_map[op_token.type];
}

void Graph::operator=(const char *expression) { parse(expression); }

void Graph::parse(string expression) {
  ExScanner scanner(expression);
  ExParser parser(scanner.scan());
  buildGraph(parser.parse());
  root->finished();
}

void Graph::buildGraph(ExNode *ex_root) {
  deque<Node *> temp_nodes;

  ex_root->reverse_iterate([&](ExNode *ex_node) {
    string value = ex_node->token.value;

    switch (ex_node->type) {
    case ExType::NAME: {
      if (nodes_map.find(value) == nodes_map.end())
        error::report("Graph", "parameter not declared", value, 0);
      temp_nodes.push_back(&nodes_map[value]);
      break;
    }
    case ExType::NUMBER: {
      if (nodes_map.find(value) == nodes_map.end())
        nodes_map[value] = Node::Scalar(stod(ex_node->token.value));
      temp_nodes.push_back(&nodes_map[value]);
      break;
    }
    case ExType::SUBGRAPH: {
      if (subgraphs_map.find(value) == subgraphs_map.end())
        error::report("Graph", "subgraph not declared", value, 0);
      if (subgraphs_map[value] == nullptr)
        error::report(
            "Graph",
            "subgraph not defined (please provide a pointer to the subgraph)",
            value, 0);
      temp_nodes.push_back(subgraphs_map[value]);
      break;
    }
    case ExType::CONSTANT: {
      temp_nodes.back()->constant();
      break;
    }
    case ExType::FUNCTION: {
      Node *op =
          createOperator(value, temp_nodes[0],
                         temp_nodes.size() >= 2 ? temp_nodes[1] : nullptr);
      temp_nodes.clear();
      temp_nodes.push_back(op);
      break;
    }
    case ExType::SYMBOL: {
      Node *op =
          createOperator(getOperatorName(ex_node->token), temp_nodes[0],
                         temp_nodes.size() >= 2 ? temp_nodes[1] : nullptr);
      temp_nodes.clear();
      temp_nodes.push_back(op);
      break;
    }
    default: {
      break;
    }
    }
  });

  root = temp_nodes[0];
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
    error::report("Graph", "parameter not declared", node_name, 0);
  return nodes_map[node_name].getGrad();
}

Graph &Graph::print() {
  root->print();
  return *this;
}
