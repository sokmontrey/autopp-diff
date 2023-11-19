#pragma once 

//TODO: constructor with initial node values
//TODO: allow sub graph
//TODO: allow number for operator that require constant for example the Pow operator takes a node and a double

namespace nodeflow {

class Graph{
private:
    std::map<std::string, Node> node_map;
    std::vector<Node*> operator_nodes;
    Node* f = nullptr;

    Node* parse(std::string s){
        if (s == "") return nullptr;

        std::string operator_name = getOperatorName(s);
        std::vector<std::string> operator_args = splitArgs(getOperatorArgs(s));

        return operator_name == "" 
            ? createNode(s) 
            : createOperator(
                parse(operator_args[0]),
                parse(operator_args[1]),
                operator_name
            );
    }
    Node* createNode(std::string node_name){
        bool is_number = isNumber(node_name);
        std::string s = (is_number ? "c-" : "") + node_name;

        if(this->node_map.find(s) == this->node_map.end()){
            //if node is not already exist
            this->node_map[s] = is_number 
                ? Node::ScalarConst(std::stod(replaceMathConstants(node_name))) 
                : Node();
        }
        return &this->node_map[s];
    }
    Node* createOperator(Node* a, Node* b, std::string operator_name){
        //TODO: check for two-nodes operator if b is exist or not then throw error 
        if (operator_name == "add"){
            this->operator_nodes.push_back(new Add(a, b));
        } else if ( operator_name == "mul" || operator_name == "matmul" || operator_name == "dot" ) {
            this->operator_nodes.push_back(new Mul(a, b));
        }else if (operator_name=="sin") {
            this->operator_nodes.push_back(new Sin(a));
        }
        return this->operator_nodes[this->operator_nodes.size() - 1];
    }
public:
    Graph(std::string s){
        this->f = parse(removeSpaces(s));
        this->f->finished();
    }
    Graph(std::string s, std::map<std::string, Node> node_map){
        this->node_map = node_map;
        this->f = parse(removeSpaces(s));
        this->f->finished();
    }
    ~Graph(){
        for (int i = 0; i < operator_nodes.size(); i++){
            delete this->operator_nodes[i];
        }
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

