#pragma once

#include <string>
#include <vector>

#include "../util/error.hpp"
#include "ex_token.hpp"

namespace nodeflow {

class ExScanner {
public:
  ExScanner(std::string str);
  ~ExScanner() = default;

  std::vector<Token> scan();

  void printTokens();
  std::vector<Token> getTokens();

private:
  std::string str;
  std::vector<Token> tokens;
  int start = 0;
  int pos = 0;

  bool isAtEnd();
  bool isDigit(char c);
  bool isAlpha(char c);

  void scanToken();

  char advance();
  char peek();
  char peek(int n);

  void addToken(TokenType type);
  void addToken(TokenType type, std::string value);

  void number();
  void word();

};

} //namespace nodeflow
