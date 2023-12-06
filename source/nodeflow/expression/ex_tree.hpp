#ifndef NODEFLOW_EXPRESSION_EX_TREE_HPP_
#define NODEFLOW_EXPRESSION_EX_TREE_HPP_

#include <vector>
#include <string>

namespace nodeflow {

enum class ExType{
  CONSTANT,
  VARIABLE,
  NUMBER,
  FUNCTION,
};

struct ExNode {
  ExType type;
  std::string value;
  std::vector<ExNode*> args;
};

} // namespace nodeflow

#endif // NODEFLOW_EXPRESSION_EX_TREE_HPP_