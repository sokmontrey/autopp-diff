#pragma once

#include <iostream>
#include <string>

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

  COMMA,
  END,
};

struct Token {
  TokenType type;
  std::string value;
  int pos;

  Token(TokenType type, std::string value, int pos)
      : type(type), value(value), pos(pos) {}
  Token() : type(END), value(""), pos(0) {}

  void print() { std::cout << value << " "; }
};

} // namespace nodeflow
