#pragma once 

namespace nodeflow {

enum TokenType {
    NUMBER,
    WORD,

    HASH,
    DOLLAR,

    OPEN,
    CLOSE,

    NEG,
    ADD,
    SUB,
    MUL,
    DIV,
    POW,

    END,
};

struct Token{
    TokenType type;
    std::string value;
    int pos;
};

std::map<std::string, TokenType> binary_operators{
    {"+", ADD},
    {"-", SUB},
    {"*", MUL},
    {"/", DIV},
    {"^", POW},
};
std::map<std::string, TokenType> unary_operators{
    {"-", NEG},
};

} // namespace nodeflow
