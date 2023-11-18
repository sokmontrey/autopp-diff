#pragma once 

#include <string>
#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <map>

namespace nodeflow {

class Graph{
private:
    std::map<std::string, Node> node_map;
    std::vector<Node*> operator_nodes;

public:
    Graph(std::string s){
        parser(s);
    }
    
    Node* parser(std::string s){
        std::string operator_name = getOperatorName(s);
        std::vector<std::string> operator_args = splitArgs(getOperatorArgs(s));

        if (operator_name == ""){
            node_map[s] = Node();
            return &node_map[s];
        } 

        return createOperator(
            parser(operator_args[0]), 
            operator_args.size() > 1 ? parser(operator_args[1]) : nullptr,
            operator_name
        );
    }

    Node* createOperator(Node* a, Node* b, std::string operator_name){
        if (operator_name == "add"){
            Node* op = new Add(a, b);
            this->operator_nodes.push_back(op);
            return op;
        } else if ( operator_name == "mul" || operator_name == "matmul" || operator_name == "dot" ) {
            Node* op = new Mul(a, b);
            this->operator_nodes.push_back(op);
            return op;
        }
    }

    std::string getOperatorName(std::string s){
        std::string temp="";
        for(int i = 0; i < s.length(); i++){
            if (s[i] == '('){
                return temp;
            }
            temp += s[i];
        }
        return "";
    }
    std::string getOperatorArgs(std::string s){
        std::string temp="";
        bool bracket_found = false;
        int bracket_count = 0;

        for(int i = 0; i < s.length(); i++){
            if ( !bracket_found && s[i] == '('){
                bracket_found = true;
                continue;
            } else if (bracket_found) {
                if (s[i] == '('){
                    bracket_count++;
                } else if (s[i] == ')'){
                    if (bracket_count == 0){
                        return temp;
                    }
                    bracket_count--;
                }
                temp += s[i];
            }
        }
        return temp;
    }
    std::vector<std::string> splitArgs(std::string s){
        //reverse for loop the string
        std::string first_args_temp = "";
        std::string second_args_temp = "";
        std::string temp;

        int bracket_count = 0;
        bool comma_found = false;

        for (int i = s.length() - 1; i >= 0; i--){
            if (s[i]==')'){
                bracket_count++;
            } else if (s[i]=='('){
                bracket_count--;
            } else if (s[i] == ',' && bracket_count == 0 && !comma_found){
                second_args_temp = temp;
                comma_found = true;
                temp = "";
                continue;
            }
            temp += s[i];
        }
        first_args_temp = temp;
        if (!comma_found) second_args_temp = "";

        std::string first_args = reverseString(first_args_temp);
        std::string second_args = reverseString(second_args_temp);

        return {first_args, second_args};
    }
    std::string reverseString(std::string s){
        std::string temp;
        for(int i = s.length() - 1; i >= 0; i--){
            temp += s[i];
        }
        return temp;
    }
};

} //namespace nodeflow

