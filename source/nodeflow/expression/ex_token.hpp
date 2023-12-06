#ifndef NODEFLOW_EXPRESSION_TOKEN_HPP_
#define NODEFLOW_EXPRESSION_TOKEN_HPP_

#include <string>
#include <iostream>

namespace nodeflow {

enum TokenType {
  NUMBER,
  WORD,

  HASH,
  DOLLAR,

  OPEN,
  CLOSE,

  PLUS,
  MINUS,
  STAR,
  SLASH,
  POW,

  SUB,
  NEG,
  INVS,

  PI,
  E,

  COMMA,
  END,
};

struct Token {
  TokenType type;
  std::string value;
  int pos;

  Token(TokenType type, std::string value, int pos)
      : type(type), value(value), pos(pos) {}

  void print() { std::cout << value << " "; }
};

} // namespace nodeflow

#endif // NODEFLOW_EXPRESSION_TOKEN_HPP_