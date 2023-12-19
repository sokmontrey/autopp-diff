#pragma once

#include "../parser/ex_parser.hpp"
#include "../parser/ex_scanner.hpp"
#include "../parser/ex_token.hpp"
#include "../util/error.hpp"
#include "./node.hpp"
#include "./operator.hpp"
#include <Eigen/Dense>
#include <deque>
#include <string>
#include <unordered_map>

namespace nodeflow {

// TODO: deallocate ExNode tree after
typedef std::function<Node *(Node *)> OneArgOpCreator;
typedef std::function<Node *(Node *, Node *)> TwoArgsOpCreator;

class Graph {
public:
  std::unordered_map<std::string, OneArgOpCreator> one_arg_ops_map{
      {"sqrt", [](Node *a) { return new Sqrt(a); }},
      {"invert", [](Node *a) { return new Invert(a); }},
      {"inverse", [](Node *a) { return new Inverse(a); }},
      {"sin", [](Node *a) { return new Sin(a); }},
      {"cos", [](Node *a) { return new Cos(a); }},
      {"tan", [](Node *a) { return new Tan(a); }},
      {"sinh", [](Node *a) { return new Sinh(a); }},
      {"cosh", [](Node *a) { return new Cosh(a); }},
      {"tanh", [](Node *a) { return new Tanh(a); }},
      {"exp", [](Node *a) { return new Exp(a); }},
      {"loge", [](Node *a) { return new Loge(a); }},
      {"ln", [](Node *a) { return new Loge(a); }},
      {"sum", [](Node *a) { return new Sum(a); }},
      {"sigmoid", [](Node *a) { return new Sigmoid(a); }},
      {"relu", [](Node *a) { return new ReLU(a); }},
      {"softmax", [](Node *a) { return new Softmax(a); }},
  };

  std::unordered_map<std::string, TwoArgsOpCreator> two_args_ops_map{
      {"add", [](Node *a, Node *b) { return new Add(a, b); }},
      {"sub", [](Node *a, Node *b) { return new Subtract(a, b); }},
      {"subtract", [](Node *a, Node *b) { return new Subtract(a, b); }},
      {"mul", [](Node *a, Node *b) { return new Mul(a, b); }},
      {"matmul", [](Node *a, Node *b) { return new Mul(a, b); }},
      {"dot", [](Node *a, Node *b) { return new Mul(a, b); }},
      {"ele_mul", [](Node *a, Node *b) { return new EleWiseMul(a, b); }},
      {"scl_mul", [](Node *a, Node *b) { return new ScalarMul(a, b); }},
      {"div", [](Node *a, Node *b) { return new Div(a, b); }},
      {"ele_div", [](Node *a, Node *b) { return new EleWiseDiv(a, b); }},
      {"scl_div", [](Node *a, Node *b) { return new ScalarDiv(a, b); }},
      {"pow", [](Node *a, Node *b) { return new Pow(a, b); }},
      {"leaky_relu", [](Node *a, Node *b) { return new LeakyReLU(a, b); }},
  };

  Graph(std::string expression,
        std::unordered_map<std::string, Node> nodes_map = {},
        std::unordered_map<std::string, Graph &> subgraph_nodes_map = {})
      : expression(expression), nodes_map(nodes_map) {
    for (auto &subgraph : subgraph_nodes_map) {
      subgraph.second.subgraph();
      this->subgraph_nodes_map.insert(
          {subgraph.first, subgraph.second.getRoot()});
    }
    parse();
  }

  ~Graph() {
    if (!is_subgraph)
      destroy();
  }

  // IMPORTANT: use this things.
  // I ran out of idea to make thing easy for you
  // so do it yourself.
  void destroy();
  void subgraph();

  Graph &finished();
  Node *getRoot();

  const Eigen::MatrixXd &evaluate(bool is_reset = true);
  Graph &reset();

  Graph &gradient(bool is_reset = true);
  const Eigen::MatrixXd &partial(std::string node_name);

  Node *node(std::string node_name);
  double value(std::string node_name, int row = 0, int col = 0);
  Eigen::MatrixXd &matrix(std::string node_name);

  Graph &setNode(std::string node_name, Node node) {
    nodes_map[node_name] = node;
    return *this;
  }

private:
  std::unordered_map<TokenType, std::string> ops_symbol_map{{PLUS, "add"},
                                                            {MINUS, "subtract"},
                                                            {SLASH, "div"},
                                                            {STAR, "mul"},
                                                            {POW, "pow"}};

  bool is_subgraph = false;

  void parse();
  void buildGraph(ExNode *ex_root);
  std::string getOperatorName(Token op_token);

  std::string expression;
  Node *root;
  std::unordered_map<std::string, Node *> subgraph_nodes_map;
  std::unordered_map<std::string, Node> nodes_map;
};

} // namespace nodeflow
