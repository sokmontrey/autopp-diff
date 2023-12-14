#pragma once

#include "../parser/ex_parser.hpp"
#include "../parser/ex_scanner.hpp"
#include "../parser/ex_token.hpp"
#include "../util/error.hpp"
#include "./node.hpp"
#include "./operator.hpp"
#include <Eigen/Dense>
#include <deque>
#include <memory>
#include <string>
#include <unordered_map>

using namespace std;

namespace nodeflow {

// TODO: deallocate ExNode tree after
typedef function<Node *(Node *)> OneArgOpCreator;
typedef function<Node *(Node *, Node *)> TwoArgsOpCreator;

class Graph {
public:
  template <typename... Args> Graph(Args... params) {
    processParams(params...);
  }

  // create node name
  Graph &declare(string node_name);
  // define node value
  Graph &define(string node_name, Node node);
  // define subgraph
  Graph &define(string subgraph_name, Graph &subgraph);

  void parse(string expression);
  void operator=(const char *expression);

  Graph &evaluate();
  Graph &operator()();

  Graph &gradient();

  Eigen::MatrixXd partial(string node_name);

  Node *getRoot();

  Graph &print();

private:
  unordered_map<string, OneArgOpCreator> one_arg_ops_map{
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

  unordered_map<string, TwoArgsOpCreator> two_args_ops_map{
      {"add", [](Node *a, Node *b) { return new Add(a, b); }},
      {"sub", [](Node *a, Node *b) { return new Subtract(a, b); }},
      {"subtract", [](Node *a, Node *b) { return new Subtract(a, b); }},
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

  unordered_map<TokenType, string> ops_symbol_map{{PLUS, "add"},
                                                  {MINUS, "subtract"},
                                                  {SLASH, "div"},
                                                  {STAR, "mul"},
                                                  {POW, "pow"}};

  void processParams(string param) { declare(param); }
  template <typename... Args> void processParams(string param, Args... params) {
    declare(param);
    processParams(params...);
  }

  void buildGraph(ExNode *ex_root);

  string getOperatorName(Token op_token);

  unordered_map<string, Node *> subgraph_nodes_map;
  unordered_map<string, Node> nodes_map;
  Node *root;
  string expression;
};

} // namespace nodeflow

/*
 * Graph f("a", "b", "$c");
 *
 * f.define("b", &b); //subgraph pointer to OperatorNode
 * f.define("c", new Node::Random(2,3)); //node value itself pointer to Node
 * list of definitions is also possible
 *
 * f.parse("a + sin(b) * cos(c)");
 * or
 * f = "a + sin(b) * cos(c)";
 *
 * evaluate
 * f.evaluate(); or f();
 *
 * derivative
 * f.gradient();
 *
 * partial derivative
 * f.partial("a");
 *
 * f["a"] or f.node("a")
 * return a reference to the node
 *
 * Graph::getVars(f);
 * Graph::getConsts(f);
 *
 */
