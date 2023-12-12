#pragma once

#include "../util/error.hpp"
#include "ex_token.hpp"
#include <string>
#include <vector>

namespace nodeflow {

enum class ExType { SYMBOL, FUNCTION, VARIABLE, CONSTANT, SUBGRAPH, END };

struct ExNode {
  ExType type;
  Token token;
  std::vector<ExNode *> childrens;
  void print();
  void deleteChildrens();
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
  ~ExParser();
  ExNode *parse();

private:
  std::vector<Token> tokens;
  int pos = 0;
  ExNode *root;

  // typedef std::function<Node *(Node *, Node *)> OpCreatorFunc;
  // std::unordered_map<std::string, OpCreatorFunc> ops_map{
  //     {"add", [](Node *a, Node *b) { return new Add(a, b); }},
  //     {"sub", [](Node *a, Node *b) { return new Subtract(a, b); }},
  //     {"sin", [](Node *a, Node *b) { return new Sin(a, b); }}};

  // std::unordered_map<TokenType, std::string> symb_op_map{
  //     {PLUS, "add"}, {STAR, "mul"},   {SLASH, "div"},      {POW, "pow"},
  //     {WORD, "var"}, {HASH, "const"}, {DOLLAR, "subgraph"}};

  ExNode *expression();
  ExNode *term();
  ExNode *factor();
  ExNode *power();
  ExNode *unary();
  ExNode *reciprocal();
  ExNode *primary();
  ExNode *function();
  ExNode *literal();

  // std::string getOpName(Token op);
  // ExNode *createOperator(std::string op_name, ExNode *a, ExNode *b);

  Token advance();
  Token peek();
  Token previous();
  Token current();

  bool isAtEnd();
  bool match(std::initializer_list<TokenType> types);
  bool matchValue(std::initializer_list<std::string> values);
};

} // namespace nodeflow
