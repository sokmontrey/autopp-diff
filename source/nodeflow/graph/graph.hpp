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

// TODO: deallocate ExNode tree after
typedef function<Node *(Node *, Node *)> OpCreatorLambda;

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
  Graph &define(string node_name, Node *node);
  void parse(string expression);
  void operator=(const char *expression);

  Graph &evaluate();
  Graph &operator()();

  Graph &gradient();

  Eigen::MatrixXd partial(string node_name);

  Graph &print();

private:
  unordered_map<string, OpCreatorLambda> ops_name_map{
      {"add", [](Node *a, Node *b) { return new Add(a, b); }},
      {"sub", [](Node *a, Node *b) { return new Subtract(a, b); }}};

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
  Node *createOperator(string op_name, Node *a, Node *b);
  string getOperatorName(Token op_token);

  unordered_map<string, Node *> subgraphs_map;
  unordered_map<string, Node> nodes_map;
  Node *root;
};

} // namespace nodeflow
