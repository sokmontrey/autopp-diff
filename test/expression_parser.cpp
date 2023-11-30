#include <nodeflow/nodeflow.h>

using namespace nodeflow;

// enum TokenType{
//     OPERATOR,
//     VARIABLE,
//     CONSTANT,
//     SUBGRAPH
// };
//
// struct Token{
//     TokenType type;
//     std::string value;
// };

bool isOperator(char c){
    return (c < 'a' || c > 'z') 
        && (c < 'A' || c > 'Z') 
        && c != '#'
        && c != '$' ;
}

std::vector<std::string> split(std::string s){
    std::vector<std::string> arr;
    std::string temp = "";

    for(auto c : s){
        if (isOperator(c)){
            temp!="" ? arr.push_back(temp) : 0;
            arr.push_back(std::string(1, c));
            temp = "";
        }else{
            temp += c;
        }
    }

    return arr;
}

std::string operatorSymbolToName(std::string s){
    if (s == "+") return "add";
    if (s == "-") return "sub";
    if (s == "*") return "mul";
    if (s == "/") return "div";
    if (s == "^") return "pow";
    return "";
}

int getOperatorPriority(std::string s){
    if (s == "+" || s == "-") return 3;
    if (s == "*" || s == "/") return 2;
    if (s == "^") return 1;
    return 0;
} 

// Symbol function
// op ( something ) + something: 
//    ( something ) + something: insert '(' and ')' around, and insert 'op' in front
//
// String function
// op ( something ): check inner
//    ( something ): remove '(' and ')' then use Symbol function
// std::string expression_tree(std::vector<std::string>& ex){
//     bool is_start_bracket = ex[0] == "(";
//     bool is_end_bracket   = ex[ex.size()-1] == ")";
//
//     if (is_start_bracket && is_end_bracket) {
//         // Just an expression with "empty" bracket around: ( something )
//         return expression_tree(std::vector<std::string>(ex.begin()+1, ex.end()-1));
//     }else if (!is_start_bracket && is_end_bracket) {
//         // A single function expression: operator( something )
//         return ex[0]+"("+expression_tree(std::vector<std::string>(ex.begin()+2, ex.end()-1))+")";
//     }
//
//     // A single variable 
//     if (ex.size() == 1) return ex[0];
//
//     // A symbol expression
//     int bracket_count = 0;
//
//     //TODO: check highest priority operator
//     //go through all the operator outside the bracket
//     //keep track of the highest priority operator and its index
//     //if there there is no operator return ex
//
//     int i = 0;
//     int highest_priority = 0;
//     int highest_priority_index = 0;
//     for(auto s : ex){
//         if (s == "(") bracket_count++;
//         if (s == ")") bracket_count--;
//         if (bracket_count == 0 && isOperator(s[0])){
//             int priority = 0;
//             priority = getOperatorPriority(s);
//             if (priority > highest_priority){
//                 highest_priority = priority;
//                 highest_priority_index = i;
//             }
//         }
//         i++;
//     }
//
//     // No operator found
//     if (highest_priority == 0) return ex[0];
//
//     i = highest_priority_index;
//
//     auto first_arg = expression_tree(std::vector<std::string>(ex.begin(), ex.begin()+i));
//     auto second_arg = expression_tree(std::vector<std::string>(ex.begin()+i+1, ex.end()));
//     std::string op = operatorSymbolToName(ex[i]);
//
//     return op+"("+first_arg+","+second_arg+")";
// }

std::string expression_tree(std::vector<std::string>& ex, int begin, int end){
    bool is_start_bracket = ex[begin] == "(";
    bool is_end_bracket   = ex[end] == ")";

    if (is_start_bracket && is_end_bracket) {
        // Just an expression with "empty" bracket around: ( something )
        return expression_tree(ex, begin+1, end-1);
    }else if (!is_start_bracket && is_end_bracket) {
        // A single function expression: operator( something )
        return ex[begin]+"("+expression_tree(ex, begin+2, end-1)+")";
    }

    // A single variable 
    if (begin == end) return ex[begin];

    // A symbol expression
    int bracket_count = 0;

    //TODO: check highest priority operator
    //go through all the operator outside the bracket
    //keep track of the highest priority operator and its index
    //if there there is no operator return ex

    int i = begin;
    int highest_priority = 0;
    int op_index = begin;
    for (i = begin; i <= end; i++){
        if (ex[i] == "(") bracket_count++;
        if (ex[i] == ")") bracket_count--;
        if (bracket_count == 0 && isOperator(ex[i][0])){
            int priority = 0;
            priority = getOperatorPriority(ex[i]);
            if (priority > highest_priority){
                highest_priority = priority;
                op_index = i;
            }
        }
    }

    // No operator found
    if (highest_priority == 0) return ex[begin];

    auto first_arg = expression_tree(ex, begin, op_index-1);
    auto second_arg = expression_tree(ex, op_index+1, end);
    std::string op = operatorSymbolToName(ex[op_index]);

    return op+"("+first_arg+","+second_arg+")";
}

std::string exParser(std::string ex){
    std::vector<std::string> token_list = split(removeSpaces(ex));

    return expression_tree(token_list, 0, token_list.size()-1);
}

int main(){

    std::cout << exParser("sin(cos((abc + #BCC) * $c + 1)) / 2") << std::endl;

    return 0;
}
