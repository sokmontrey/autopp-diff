#include "ex_parser.hpp"
#include "ex_token.hpp"

using namespace nodeflow;

void ExNode::print(std::string indent) {
  // print in form + (a, b)
  std::cout << token.value;
  if (childrens.size() > 0)
    std::cout << "(\n";

  for (int i = 0; i < childrens.size(); i++) {
    std::cout << indent + "  ";
    childrens[i]->print(indent + "  ");
    if (i < childrens.size() - 1)
      std::cout << ",\n";
  }

  if (childrens.size() > 0)
    std::cout << "\n" + indent + ")";
}

void ExNode::deleteChildrens() {
  reverse_iterate([](ExNode *ex_node) { delete ex_node; });
}

void ExNode::reverse_iterate(std::function<void(ExNode *)> func) {
  for (auto child : childrens) {
    child->reverse_iterate(func);
  }
  func(this);
}

//=============================================================================
//                                  ExParser
//=============================================================================

ExParser::ExParser(std::vector<Token> tokens) { this->tokens = tokens; }
ExParser::~ExParser() {
  if (root != nullptr) {
    root->deleteChildrens();
    delete root;
  }
}

ExNode *ExParser::parse() { return expression(); }
ExNode *ExParser::expression() {
  root = term();
  return root;
}

ExNode *ExParser::term() {
  ExNode *expr = factor();
  while (match({MINUS, PLUS})) {
    Token op = advance();
    ExNode *right = factor();
    expr = new ExNode{ExType::SYMBOL, op, _ExArgs{expr, right}};
  }
  return expr;
}

ExNode *ExParser::factor() {
  ExNode *expr = power();
  while (match({SLASH, STAR})) {
    Token op = advance();
    ExNode *right = power();
    expr = new ExNode{ExType::SYMBOL, op, _ExArgs{expr, right}};
  }
  return expr;
}

ExNode *ExParser::power() {
  ExNode *expr = unary();
  while (match({POW})) {
    Token op = advance();
    ExNode *right = unary();
    expr = new ExNode{ExType::SYMBOL, op, _ExArgs{expr, right}};
  }
  return expr;
}

ExNode *ExParser::unary() {
  if (match({MINUS, HASH, DOLLAR})) {
    Token op(advance());
    ExNode *expr = unary();

    if (op.type == MINUS)
      op.value = "invert";

    ExType type = ExType::FUNCTION;
    if (op.type == HASH)
      type = ExType::CONSTANT;
    else if (op.type == DOLLAR)
      type = ExType::SUBGRAPH;

    expr = new ExNode{type, op, _ExArgs{expr}};
    return expr;
  }
  return reciprocal();
}

ExNode *ExParser::reciprocal() {
  if (matchValue({"1"}) && peek().type == SLASH) {
    advance();
    advance();
    ExNode *expr = reciprocal();
    return new ExNode{ExType::FUNCTION, Token(SLASH, "inverse", 0),
                      _ExArgs{expr}};
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

    _ExArgs args;
    while (!match({CLOSE}) && !isAtEnd()) {
      args.push_back(expression());
      if (match({COMMA}))
        advance();
    }

    if (!match({CLOSE})) {
      error::report("ExpressionParser", "Expected ')' after arguments",
                    previous().value, 2);
      return nullptr;
    }

    advance();
    return new ExNode{ExType::FUNCTION, op, args};
  }

  return literal();
}
ExNode *ExParser::literal() {
  if (match({NUMBER, WORD})) {
    Token op(advance());
    if (op.type == NUMBER) {
      return new ExNode{ExType::NUMBER, op, _ExArgs()};
    }
    return new ExNode{ExType::NAME, op, _ExArgs()};
  }
  error::report("ExpressionParser", "Expected number or word",
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
