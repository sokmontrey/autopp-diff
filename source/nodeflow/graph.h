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
    Node* f = nullptr;

public:
    Graph(std::string s){
        this->f = parser(removeSpaces(s));
        this->f->finished();
    }
    ~Graph(){
        for (int i = 0; i < operator_nodes.size(); i++){
            delete this->operator_nodes[i];
        }
    }
    
    Node* parser(std::string s){
        std::string operator_name = getOperatorName(s);
        std::vector<std::string> operator_args = splitArgs(getOperatorArgs(s));

        if (operator_name == ""){
            if(this->node_map.find(s) == this->node_map.end()){
                this->node_map[s] = Node();
            }
            return &this->node_map[s];
        } 

        createOperator(
            parser(operator_args[0]), 
            operator_args.size() > 1 ? parser(operator_args[1]) : nullptr,
            operator_name
        );

        return this->operator_nodes[this->operator_nodes.size() - 1];
    }
    void createOperator(Node* a, Node* b, std::string operator_name){
        if (operator_name == "add"){
            this->operator_nodes.push_back(new Add(a, b));
        } else if ( operator_name == "mul" || operator_name == "matmul" || operator_name == "dot" ) {
            this->operator_nodes.push_back(new Mul(a, b));
        }else if (operator_name=="sin") {
            this->operator_nodes.push_back(new Sin(a));
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
    std::string removeSpaces(std::string s){
        std::string temp="";
        for(int i = 0; i < s.length(); i++){
            if (s[i] != ' '){
                temp += s[i];
            }
        }
        return temp;
    }
    //a method that convert symbol expression to function expression
    //for example: a + b -> add(a, b)
    // implement this method for +, -, *, /, ^ and ()
    // for example: a + b * c -> add(a, mul(b, c))
    // for example: (a + b) * c -> mul(add(a, b), c)
    // if there are already functions in the expression, only convert the symbols to functions
    // for example: add(a, b) + c -> add(add(a, b), c)
    std::string symbolToFunction(std::string s){

        return s;
    }

    Graph& setNode(std::string name, Node node){
        this->node_map[name] = node;
        return *this;
    }
    Node* getNode(std::string name){
        return &this->node_map[name];
    }
    Eigen::MatrixXd getGrad(std::string name){
        return this->node_map[name].getGrad();
    }
    Eigen::MatrixXd getValue(std::string name){
        return this->node_map[name].getValue();
    }
    Eigen::MatrixXd get(std::string name){
        return this->node_map[name].getValue();
    }
    Eigen::MatrixXd getOutput(){
        return this->f->getValue();
    }
    Graph& setValue(std::string name, Eigen::MatrixXd value){
        this->node_map[name].setValue(value);
        return *this;
    }
    Graph& set(std::string name, Eigen::MatrixXd value){
        this->node_map[name].setValue(value);
        return *this;
    }

    Node* getF(){
        return this->f;
    }

    Graph& constant(std::string name){
        this->node_map[name].constant();
        return *this;
    }

    Graph& finished(){
        this->f->finished();
        return *this;
    }

    Graph& forward(){
        this->f->forward();
        return *this;
    }

    Graph& backward(){
        this->f->backward();
        return *this;
    }

    Graph& print(){
        this->f->print();
        return *this;
    }

    Graph& reset(){
        this->f->reset();
        return *this;
    }
};

} //namespace nodeflow

