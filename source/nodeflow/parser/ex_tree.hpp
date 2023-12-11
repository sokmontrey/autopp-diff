#pragma once

#include "ex_token.hpp"
#include <vector>

namespace nodeflow {

enum class ExType {
  CONSTANT,
  VARIABLE,
  NUMBER,
  FUNCTION,
};

struct ExNode {
  ExType type;
  Token token;
  std::vector<ExNode *> args;
  void print() {
    std::cout << token.value << (args.size() ? "(" : "");
    for (int i = 0; i < args.size(); i++) {
      args[i]->print();
      std::cout << (i == args.size() - 1 ? "" : ", ");
    }
    std::cout << (args.size() ? ")" : "");
  }
};

} // namespace nodeflow
