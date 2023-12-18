#pragma once

#include <Eigen/Dense>

#include "../util/error.hpp"
#include <functional>
#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

namespace nodeflow {
/**
 * A Node class that is used to build a graph.
 *
 * @constructors:
 *      @params: Eigen::Matrix or Eigen::Vector : Assign an Eigen::Matrix or
 * Vector object to the value member.
 */

enum class NodeType { NODE, OPERATOR };

class Node {
protected:
  bool is_differentiatable = true;
  bool is_constant = false;
  bool is_value_ready = false;
  std::string name = "";

  Eigen::MatrixXd value;
  Eigen::MatrixXd outer_derivative;

  unsigned int rows = 0;
  unsigned int cols = 0;

  int num_parent = 0;
  int parent_called_count = 0;

  void setRows(size_t rows);
  void setCols(size_t cols);

public:
  Node() = default;
  Node(Eigen::MatrixXd initial_value);

  virtual void reverse_iterate(std::function<void(Node *)> func);

  //================================================================
  //                          Getters
  //================================================================

  size_t getRows();
  size_t getCols();
  Eigen::MatrixXd &operator()();
  double &operator()(size_t row, size_t col);
  double getValue(size_t row, size_t col);
  double getValue();
  Eigen::MatrixXd &getMatrix();
  Eigen::MatrixXd &getGrad();
  std::string getName();
  virtual NodeType getType();

  //================================================================
  //                          Setters
  //================================================================

  void setVector(Eigen::VectorXd new_value);
  void setVector(std::initializer_list<double> list);

  void setMatrix(Eigen::MatrixXd new_value);
  void setMatrix(std::initializer_list<std::initializer_list<double>> list);

  void setValue(size_t row, size_t col, double new_value);
  void setValue(double new_value);
  void setName(std::string new_name);
  Node &operator=(Eigen::MatrixXd new_value);

  //================================================================
  //                       Node Methods
  //================================================================

  bool isDifferentiatable();
  bool isConstant();
  Node &constant();

  //================================================================
  //                       Graph Methods
  //================================================================

  virtual std::vector<Node *> getAllLeaveNode();

  virtual void finished();
  virtual void finished(bool is_child);
  void clearGraph();
  virtual void reset();
  virtual Eigen::MatrixXd &forward();
  virtual void backward(Eigen::MatrixXd partial_outer_derivative);
  virtual void backward();

  //================================================================
  //                        Scalar Methods
  //================================================================

  static Node Random();
  static Node Scalar(double initial_value);
  static Node ScalarConst(double initial_value);

  //================================================================
  //                        Vector Methods
  //================================================================

  static Node Random(size_t rows);
  static Node Vector(size_t rows);
  static Node Vector(size_t rows, double fill_value);
  static Node Vector(std::initializer_list<double> initial_vector);

  //================================================================
  //                        Matrix Methods
  //================================================================

  static Node Random(size_t rows, size_t cols);
  static Node Matrix(size_t rows, size_t cols);
  static Node Matrix(size_t rows, size_t cols, size_t fill_value);
  static Node
  Matrix(std::initializer_list<std::initializer_list<double>> initial_matrix);
};

/**
 * Based class for Operator Nodes.
 *
 * @methods:
 * .finished() must be used at the final OperatorNode to finish setting up the
 * graph .forward() is for forward evaluation of the function Any parameters for
 * the function has to be set directly to the input Node This can be done using
 * node_name = Eigen::Matrix OR Eigen::Vector OR node_name.getValue(row, col) =
 * new_element_value OR node_name(row, col) = new_element_value .backward()
 * propagate backward to calculate partial derivative of the function with
 * respect to each input Nodes Can be called independanly from .forward(). Does
 * not need to call .foward() to operate .backward() .reset() must be used after
 * each .forward() or .backward() call
 * @constructors:
 *      1. @params: input_list: initializer_list of Node* to input Node(s)
 *      2. @params: a: pointer to a single input Node
 *      3. @params: a, b: pointer to first and second input Node
 */
class OperatorNode : public Node {
protected:
  std::vector<Node *> inputs;

  size_t num_inputs = 0;
  virtual void compute() = 0;
  virtual Eigen::MatrixXd derivative(size_t input_wrt_index) = 0;
  virtual void setSize();

public:
  OperatorNode() = default;
  OperatorNode(std::initializer_list<Node *> input_list);
  OperatorNode(Node *a);
  OperatorNode(Node *a, Node *b);
  void initializeInput(std::initializer_list<Node *> input_list);
  void addInput(Node *new_input);

  void reverse_iterate(std::function<void(Node *)> func) override;

  //================================================================
  //                          Getters
  //================================================================

  Eigen::MatrixXd getInput(size_t input_index);
  Eigen::MatrixXd getInput();
  NodeType getType() override;

  //================================================================
  //                      Graph Methods
  //================================================================

  std::vector<Node *> getAllLeaveNode() override;
  void clearGraph();
  void reset() override;
  void finished();
  void finished(bool is_child) override;
  Eigen::MatrixXd &forward() override;
  void backward();
  void backward(Eigen::MatrixXd partial_outer_derivative) override;
};

} // namespace nodeflow
