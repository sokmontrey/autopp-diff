#pragma once

#include <string>
#include <vector>
#include "ex_token.hpp"
#include "ex_tree.hpp"

namespace nodeflow {

class ExParser {
public:
  ExParser(std::vector<Token> tokens);

private:
  std::vector<Token> tokens;
  int current = 0;

  ExNode *expression();
  ExNode *term();
  ExNode *factor();
  ExNode *power();
  ExNode *unary();
  ExNode *reciprocal();
  ExNode *primary();
  ExNode *function();
  ExNode *args();
  ExNode *literal();
};

} // namespace nodeflow

/*
 * expression    -> term;
 *
 * term          -> factor ( (MINUS | PLUS) factor )*;
 *
 * factor        -> power ( (SLASH | STAR) power )*;
 *
 * power         -> unary POW unary;
 *
 * unary         -> (MINUS) unary
 *               | reciprocal;
 *
 * reciprocal    -> "1" SLASH primary;
 *
 * primary       -> function
 *               | OPEN expression CLOSE
 *               | reciprocal
 *               | literal
 *               | (HASH | DOLLAR) primary;
 *
 * function      -> WORD OPEN args CLOSE;
 *
 * args          -> expression ( COMMA expression )*;
 *
 * literal       -> NUMBER | WORD | PI | E;
 */

