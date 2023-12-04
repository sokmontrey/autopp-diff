#pragma once

//TODO: add support for
//variables names;
//constants name (#), constant value, constant shotcut (pi, e, etc).
//subgraph name ($)
//floating point numbers
//nagation operator (-a)
//negative numbers (-1)
//priority operator (a+b*c)
//bracket operator (a+(b*c))
//function operator (sin(a))
//basic operators (+, -, *, /, ^)
//exponential function (e^a)

namespace nodeflow {

class Scanner{
private:
    std::string str;
    std::vector<Token> tokens;
    int start = 0;
    int pos = 0;
public:
    Scanner(std::string str);
    ~Scanner() = default;

    std::vector<Token> scan();

    bool isAtEnd();
    bool isDigit(char c);
    bool isAlpha(char c);

    void scanToken();

    char advance();
    char peek();
    char peek(int n);

    void addToken(TokenType type);

    void number();
    void word();

};

Scanner::Scanner(std::string str){ this->str = str; }

std::vector<Token> Scanner::scan(){
    while (!isAtEnd()){
        start = pos;
        scanToken();
    }
    return this->tokens;
}

void Scanner::scanToken(){
    char c = advance();
    switch (c){
        case ' ': break;
        case '(': addToken(OPEN); break;
        case ')': addToken(CLOSE); break;
        case '+': addToken(ADD); break;
        case '*': addToken(MUL); break;
        case '/': addToken(DIV); break;
        case '^': addToken(POW); break;
        case '#': addToken(HASH); break;
        case '$': addToken(DOLLAR); break;
        case '-': {
            char prev;
            for (int i=-2; (prev = peek(i)) == ' '; i--) {}

            if (prev == '\0') 
                addToken(NEG);
            else if (isDigit(prev) || isAlpha(prev) || prev == ')' || prev == '.') 
                addToken(SUB);
            else 
                addToken(NEG);
        } break;
        default: 
            if (isDigit(c) || c == '.'){
                number();
            }else if (isAlpha(c)){
                word();
            }else{
                error::report("Scanner::scanToken", "Unexpected character", str, pos);
            }
    }
}

bool Scanner::isAtEnd(){ return pos >= str.size(); }
bool Scanner::isDigit(char c){ return c >= '0' && c <= '9'; }
bool Scanner::isAlpha(char c){ 
    return (c >= 'a' && c <= 'z') 
    || (c >= 'A' && c <= 'Z')
    || c == '_';
}

char Scanner::advance(){ return str[pos++]; }
char Scanner::peek(){ 
    if (isAtEnd()) return '\0';
    return str[pos]; 
}
char Scanner::peek(int n){ 
    if (pos + n >= str.size() || pos + n < 0) return '\0';
    return str[pos + n]; 
}

void Scanner::addToken(TokenType type){
    std::string text = str.substr(start, pos - start);
    std::cout << text << std::endl;
    this->tokens.push_back(Token{type, text, start});
}
void Scanner::number(){
    while (isDigit(peek())) advance();
    if (peek() == '.' && isDigit(peek(1))){
        advance();
        while (isDigit(peek())) advance();
    }
    addToken(NUMBER);
}

void Scanner::word(){
    while (isAlpha(peek())) advance();
    addToken(WORD);
}

} // namespace nodeflow
