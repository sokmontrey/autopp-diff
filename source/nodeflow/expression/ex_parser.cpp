#include "ex_parser.hpp"

using namespace nodeflow;

ExParser::ExParser(std::vector<Token> tokens) { this->tokens = tokens; }

ExNode *ExParser::expression() { return term(); }
ExNode *ExParser::term() { return nullptr; }
ExNode *ExParser::factor() { return nullptr; }
ExNode *ExParser::power() { return nullptr; }
ExNode *ExParser::unary() { return nullptr; }
ExNode *ExParser::reciprocal() { return nullptr; }
ExNode *ExParser::primary() { return nullptr; }
ExNode *ExParser::function() { return nullptr; }
ExNode *ExParser::args() { return nullptr; }
ExNode *ExParser::literal() { return nullptr; }

