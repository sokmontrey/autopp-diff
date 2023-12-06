#pragma once


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

