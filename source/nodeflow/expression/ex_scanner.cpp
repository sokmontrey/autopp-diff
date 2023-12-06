#include "ex_scanner.hpp"

using namespace nodeflow;

ExScanner::ExScanner(std::string str) { this->str = str; }

std::vector<Token> ExScanner::scan() {
  while (!isAtEnd()) {
    start = pos;
    scanToken();
  }
  addToken(END, "");
  return this->tokens;
}

void ExScanner::scanToken() {
  char c = advance();
  switch (c) {
  case ' ':
    break;
  case '(':
    addToken(OPEN);
    break;
  case ')':
    addToken(CLOSE);
    break;
  case '+':
    addToken(PLUS);
    break;
  case '*':
    addToken(STAR);
    break;
  case '/':
    addToken(SLASH);
    break;
  case '^':
    addToken(POW);
    break;
  case '#':
    addToken(HASH);
    break;
  case '$':
    addToken(DOLLAR);
    break;
  case ',':
    addToken(COMMA);
    break;
  case '-':
    addToken(MINUS);
    break;
  // case '-': {
  //     char prev;
  //     for (int i=-2; (prev = peek(i)) == ' '; i--) {}
  //
  //     if (prev == '\0')
  //         addToken(NEG);
  //     else if (isDigit(prev) || isAlpha(prev) || prev == ')' || prev == '.')
  //         addToken(SUB);
  //     else
  //         addToken(NEG);
  // } break;
  default:
    if (isDigit(c) || c == '.') {
      number();
    } else if (isAlpha(c)) {
      word();
    } else {
      error::report("ExScanner::scanToken",
                    "Unexpected character: \"" + std::string{c} + "\"", str,
                    pos);
    }
  }
}

bool ExScanner::isAtEnd() { return pos >= str.size(); }
bool ExScanner::isDigit(char c) { return c >= '0' && c <= '9'; }
bool ExScanner::isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

char ExScanner::advance() { return str[pos++]; }
char ExScanner::peek() {
  if (isAtEnd())
    return '\0';
  return str[pos];
}
char ExScanner::peek(int n) {
  if (pos + n >= str.size() || pos + n < 0)
    return '\0';
  return str[pos + n];
}

void ExScanner::addToken(TokenType type) {
  std::string value = str.substr(start, pos - start);
  addToken(type, value);
}
void ExScanner::addToken(TokenType type, std::string value) {
  this->tokens.push_back(Token(type, value, start));
}
void ExScanner::number() {
  while (isDigit(peek()))
    advance();
  if (peek() == '.' && isDigit(peek(1))) {
    advance();
    while (isDigit(peek()))
      advance();
  }
  addToken(NUMBER);
}

void ExScanner::word() {
  while (isAlpha(peek()))
    advance();
  std::string word = str.substr(start, pos - start);
  if (word == "PI") {
    addToken(PI);
  } else if (word == "E") {
    addToken(E);
  } else {
    addToken(WORD, word);
  }
}
void ExScanner::printTokens() {
  for (auto token : tokens)
    token.print();
}
std::vector<Token> ExScanner::getTokens(){ return this->tokens; }
