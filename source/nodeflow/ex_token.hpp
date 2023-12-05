#pragma once

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

  PI,
  E,

  COMMA,
  END,
};

std::string bg_color[] = {
    "\033[91m",
    "\033[95m",

    "\033[32m",
    "\033[32m",

    "\033[0;100m\033[0;97m",
    "\033[0;100m\033[0;97m",

    "\033[0;93m",
    "\033[0;93m",
    "\033[0;93m",
    "\033[0;93m",
    "\033[0;93m",

    "\033[35m",
    "\033[35m",

    "\033[36m",
    "\033[36m",
};


struct Token {
  TokenType type;
  std::string value;
  int pos;

  Token(TokenType type, std::string value, int pos)
      : type(type), value(value), pos(pos) {}

  void print() { std::cout << bg_color[type] << value << "\033[0m "; }
};

} // namespace nodeflow
