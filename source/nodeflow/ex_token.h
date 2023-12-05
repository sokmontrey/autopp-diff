#pragma once 

namespace nodeflow {

enum TokenType {
    NUMBER, WORD,

    HASH, DOLLAR,

    OPEN, CLOSE,

    NEG, ADD, SUB, MUL, DIV, POW,

    COMMA, END,
};

std::vector<std::string> TokenNames{
    "NUMBER", "WORD",

    "HASH", "DOLLAR",

    "OPEN", "CLOSE",

    "NEG", "ADD", "SUB", "MUL", "DIV", "POW",

    "COMMA", "END",
};

struct Token{
    TokenType type;
    std::string value;
    int pos;

    Token(TokenType type, std::string value, int pos) 
    : type(type), value(value), pos(pos) { }

    void print(){
        //value     type
        printf("%-10s %s\n", value.c_str(), TokenNames[type].c_str());
    }
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
