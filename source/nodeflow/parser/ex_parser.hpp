#pragma once

#include "../util/error.hpp"
#include "ex_token.hpp"
#include <functional>
#include <string>
#include <vector>

namespace nodeflow {

enum class ExType { SYMBOL, FUNCTION, CONSTANT, SUBGRAPH, NUMBER, NAME };

struct ExNode {
  ExType type;
  Token token;
  std::vector<ExNode *> childrens;

  void print(std::string indent = "");
  void reverse_iterate(std::function<void(ExNode *)> func);
};

typedef std::vector<ExNode *> _ExArgs;

/*
 * expression    -> term;
 *
 * term          -> factor ( (MINUS | PLUS) factor )*;
 *
 * factor        -> power ( (SLASH | STAR) power )*;
 *
 * power         -> unary ( (POW) unary )* ;
 *
 * unary         -> (MINUS) unary
 *               | reciprocal;
 *
 * reciprocal    -> "1" SLASH reciprocal | primary;
 *
 * primary       -> OPEN expression CLOSE
 *               | (HASH | DOLLAR) primary;
 *               | function
 *
 * function      -> WORD OPEN expression (COMMA, expression)* CLOSE
 *               | literal;
 *
 * literal       -> NUMBER | WORD | PI | E;
 */

class ExParser {
public:
  ExParser(std::vector<Token> tokens);
  ExNode *parse();

private:
  std::vector<Token> tokens;
  int pos = 0;
  ExNode *root;

  ExNode *expression();
  ExNode *term();
  ExNode *factor();
  ExNode *power();
  ExNode *unary();
  ExNode *reciprocal();
  ExNode *primary();
  ExNode *function();
  ExNode *literal();

  Token advance();
  Token peek();
  Token previous();
  Token current();

  bool isAtEnd();
  bool match(std::initializer_list<TokenType> types);
  bool matchValue(std::initializer_list<std::string> values);
};

} // namespace nodeflow
