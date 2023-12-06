#pragma once

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
