#ifndef NODEFLOW_GRAPH_HPP
#define NODEFLOW_GRAPH_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "node.hpp"
#include "operator.hpp"
#include "../util/error.hpp"
#include "../expression/ex_scanner.hpp"
#include "../expression/ex_parser.hpp"

namespace nodeflow {

/**
 * High level class for creating a graph of nodes
 * params:
 *     s: string of math expression with composition of functions
 *     node_map: map of nodes that are used in the graph
 *     subgraphs: map of subgraphs that are used in the graph
 *
 * Expression rules:
 *      - Function names are in lower case
 *      - Argument can be variable, constant, or subgraph
 *          - Variable: any string that is not a number or a subgraphs
 *              - Example: "a", "b", "_a_1", "hello"
 *          - Constant: when an argument is a number or a math constant, it is
 * considered as a constant
 *              - Partial Derivative will not be calculated for constants
 *              - Example: "1", "3.14", "PI", "E"
 *          - Subgraph: when an argument name starts with "$", it is considered
 * as a subgraph
 *              - Example: "$subgraph_1", "$subgraph_2"
 *              - A subgraph MUST be a reference to Node or to an OperatorNode.
 *              - The reference MUST be provided in the constructor of the main
 * graph
 *                  - Example 1:
 *
 *                      Node a;
 *                      Graph g("sin($a)", {
 *                          {"$a", &a} // map for subgraphs
 *                      });
 *
 *                 - Example 2:
 *
 *                      //... define nodes a and b
 *                      Add f(&a, &b);
 *                      Graph g("sin($f)", {
 *                          {"$f", &f} // map for subgraphs
 *                      });
 *
 *
 *                  - Example 3:
 *
 *                      //... define Graph f
 *                      Graph g("sin($f)", {
 *                          // .getEnd() or .getF() can be used to get the last
 * operator node of a graph
 *                          {"$f", f.getEnd()}
 *                      });
 */

class Graph {
private:
  std::map<std::string, Node> node_map;
  std::vector<Node *> operator_nodes;
  std::map<std::string, Node *> sub_graphs;
  Node *f = nullptr;
  std::string expression;

  //================================================================
  //                          Parser
  //================================================================

  //================================================================
  //                         Initializer
  //================================================================

  void init();

public:
  //================================================================
  //                  No sub_graphs constructor
  //================================================================

  Graph(std::string s);
  Graph(std::string s, std::map<std::string, Node> node_map);

  //================================================================
  //                          No node_map
  //================================================================

  // pointer to subgraphs (end node)
  Graph(std::string s, std::map<std::string, Node *> subgraphs);
  // pointer to subgraphs
  Graph(std::string s, std::map<std::string, Graph *> subgraphs);
  // reference to subgraphs
  Graph(std::string s, std::map<std::string, Graph &> subgraphs);

  //================================================================
  //                  node_map with subgraphs
  //================================================================

  // pointer to subgraphs (end node)
  Graph(std::string s, std::map<std::string, Node> node_map,
        std::map<std::string, Node *> subgraphs);
  // pointer to subgraphs
  Graph(std::string s, std::map<std::string, Node> node_map,
        std::map<std::string, Graph *> subgraphs);
  // reference to subgraphs
  Graph(std::string s, std::map<std::string, Node> node_map,
        std::map<std::string, Graph &> subgraphs);
  ~Graph();

  //================================================================
  //                        Node Methods
  //================================================================

  Graph &constant(std::string name);
  Graph &setNode(std::string name, Node node);
  Node *getNode(std::string name);

  std::vector<std::string> getVariableName();
  std::vector<Node *> getVariableNode();
  std::vector<Node *> getAllLeaveNode();

  std::vector<Node *> getAllVariableNode();

  Node *getOperatorNode(int i);
  size_t getOperatorLength();

  //================================================================
  //                        Eigen Methods
  //================================================================
  /**
   * Methods for directly getting and setting Eigen::MatrixXd
   */

  /* getter */
  Eigen::MatrixXd getGrad(std::string name);

  double getValue(std::string name, size_t i, size_t j);
  double getValue(std::string name);
  double getValue();

  Eigen::MatrixXd getMatrix(std::string name);
  Eigen::MatrixXd getMatrix();

  /* end node getter */
  Node *getF();
  Node *getEnd(); 

  /* setter */
  Graph &setValue(std::string name, size_t i, size_t j, double value);
  Graph &setValue(std::string name, double value);
  Graph &setMatrix(std::string name, Eigen::MatrixXd value);

  //================================================================
  //                      Function Methods
  //================================================================
  /**
   * Methods that are used on the last operator (f)
   *   without having to get f from the graph
   */
  Graph &finished();
  Graph &forward();
  Eigen::MatrixXd operator()();
  Graph &backward();
  Graph &backward(Eigen::MatrixXd partial_outer_derivative);
  Graph &print();
  Graph &reset();
};

} // namespace nodeflow

#endif // NODEFLOW_GRAPH_HPP