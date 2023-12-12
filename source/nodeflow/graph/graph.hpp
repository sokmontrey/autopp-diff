#pragma once

#include "../parser/ex_parser.hpp"
#include "./node.hpp"
#include "./operator.hpp"
#include <map>
#include <string>
#include <tuple>
#include <vector>

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

class Graph {
public:
  Graph(string name, ...);
  ~Graph();

private:
  vector<Node *> sub_graphs;
  vector<Node *> nodes;
  vector<OperatorNode *> operators;
};

}; // namespace nodeflow
