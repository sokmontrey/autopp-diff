#include "ex_parser.hpp"
#include "ex_token.hpp"
#include <initializer_list>

using namespace nodeflow;

ExParser::ExParser(std::vector<Token> tokens) { this->tokens = tokens; }

ExNode *ExParser::expression() {
  root = term();
  return root;
}
ExNode *ExParser::term() {
  ExNode *expr = factor();
  while (match({MINUS, PLUS})) {
    Token op(advance());
    ExNode *right = factor();
    expr = new ExNode{ExType::FUNCTION, op, std::vector<ExNode *>{expr, right}};
  }
  return expr;
}
ExNode *ExParser::factor() {
  ExNode *expr = power();
  while (match({SLASH, STAR})) {
    Token op(advance());
    ExNode *right = power();
    expr = new ExNode{ExType::FUNCTION, op, std::vector<ExNode *>{expr, right}};
  }
  return expr;
}
ExNode *ExParser::power() {
  ExNode *expr = unary();
  while (match({POW})) {
    Token op(advance());
    ExNode *right = unary();
    expr = new ExNode{ExType::FUNCTION, op, std::vector<ExNode *>{expr, right}};
  }
  return expr;
}
ExNode *ExParser::unary() {
  if (match({MINUS, HASH, DOLLAR})) {
    Token op(advance());
    ExNode *expr = unary();
    return new ExNode{ExType::FUNCTION, op, std::vector<ExNode *>{expr}};
  }
  return reciprocal();
}
ExNode *ExParser::reciprocal() {
  if (match({NUMBER}) && matchValue({"1"}) && peek().type == SLASH) {
    // inverse
    advance();
    advance();
    Token op(SLASH, "inverse", 0);
    ExNode *expr = reciprocal();
    return new ExNode{ExType::FUNCTION, op, std::vector<ExNode *>{expr}};
  }
  return primary();
}
ExNode *ExParser::primary() {
  if (match({OPEN})) {
    advance();
    ExNode *expr = expression();
    if (!match({CLOSE})) {
      error::report("ExpressionParser", "Expected ')' after expression",
                    peek().value, 0);
    }
    advance();
    return expr;
  }

  return function();
}
ExNode *ExParser::function() {
  if (peek().type == OPEN) {
    Token op(advance());
    advance();
    std::vector<ExNode *> function_expressions;
    while (!match({CLOSE}) && !isAtEnd()) {
      function_expressions.push_back(expression());
      if (match({COMMA}))
        advance();
    }
    if (!match({CLOSE})) {
      error::report("ExpressionParser", "Expected ')' after arguments",
                    previous().value, 2);
    }
    advance();
    return new ExNode{ExType::FUNCTION, op, function_expressions};
  }

  return literal();
}
ExNode *ExParser::literal() {
  if (match({NUMBER, WORD, PI, E})) {
    Token op(advance());
    // TODO
    return new ExNode{ExType::NUMBER, op, std::vector<ExNode *>()};
  }
  error::report("ExpressionParser", "Expected literal",
                previous().value + " " + current().value, 2);
  return nullptr;
}

Token ExParser::advance() { return tokens[pos++]; }
Token ExParser::peek() { return tokens[isAtEnd() ? pos : pos + 1]; }
Token ExParser::current() { return tokens[pos]; }
Token ExParser::previous() { return tokens[pos > 0 ? pos - 1 : pos]; }

bool ExParser::isAtEnd() { return pos >= tokens.size() - 1; }
bool ExParser::match(std::initializer_list<TokenType> types) {
  if (isAtEnd())
    return false;
  for (auto type : types)
    if (type == tokens[pos].type)
      return true;
  return false;
}
bool ExParser::matchValue(std::initializer_list<std::string> values) {
  if (isAtEnd())
    return false;
  for (auto value : values)
    if (value == tokens[pos].value)
      return true;
  return false;
}
