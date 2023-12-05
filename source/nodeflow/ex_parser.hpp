/*
 * expression    -> term;
 *
 * term          -> factor ( ("-" | "+") factor )*;
 *
 * factor        -> power ( ("/" | "*") power )*;
 *
 * power         -> unary "^" unary;
 *
 * unary         -> ("-") unary
 *               | indentifer;
 *
 * identifier    -> ("#" | "$") indentifer
 *               | constant;
 *
 * constant      -> (PI | E) | primary;
 *
 * primary       -> WORD "(" expression ( (",") expression )* ")"
 *               | "(" expression ")"
 *               | "1" "/" "(" expression ")"
 *               | NUMBER
 *               | WORD ;
 */

namespace nodeflow {

class ExParser {
private:
  std::vector<Token> tokens;

public:
  ExParser(std::vector<Token> tokens);
};

ExParser::ExParser(std::vector<Token> tokens) { this->tokens = tokens; }

} // namespace nodeflow
