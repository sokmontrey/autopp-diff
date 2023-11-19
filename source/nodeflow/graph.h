#pragma once 

namespace nodeflow {

class Graph{
private:
    std::map<std::string, Node> node_map;
    std::vector<Node*> operator_nodes;
    std::map<std::string, Node*> sub_graphs;
    Node* f = nullptr;

//================================================================
//                          Parser
//================================================================

    Node* parse(std::string s){
        if (s == "") return nullptr;

        std::string operator_name = getOperatorName(s);

        std::pair<std::string,std::string> operator_args = splitArgs(getOperatorArgs(s));

        if (operator_name == "") return createNode(s);

        this->operator_nodes.push_back(createOperator(
            parse(operator_args.first),
            parse(operator_args.second),
            operator_name
        ));
        return this->operator_nodes.back();
    }
    Node* createNode(std::string node_name){
        if (node_name[0] == '$') return this->sub_graphs[node_name];

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
    Node* createOperator(Node* a, Node* b, std::string operator_name);

//================================================================
//                         Initializer
//================================================================

    void init(std::string s){
        this->f = parse(removeSpaces(s));
        this->f->finished();
    }
public:
    Graph(std::string s){
        init(s);
    }
    Graph(
        std::string s, 
        std::map<std::string, Node> node_map
    ){
        this->node_map = node_map;
        init(s);
    }
    Graph(
        std::string s, 
        std::map<std::string, Node*> subgraphs
    ){
        this->sub_graphs = subgraphs;
        init(s);
    }
    Graph(
        std::string s, 
        std::map<std::string, Node> node_map, 
        std::map<std::string, Node*> subgraphs
    ){
        this->node_map = node_map;
        this->sub_graphs = subgraphs;
        init(s);
    }
    ~Graph(){
        for (int i = 0; i < operator_nodes.size(); i++){
            delete this->operator_nodes[i];
        }
    }

//================================================================
//                        Node Methods
//================================================================

    Graph& constant(std::string name){
        this->node_map[name].constant();
        return *this;
    }
    Graph& setNode(std::string name, Node node){
        this->node_map[name] = node;
        return *this;
    }
    Node* getNode(std::string name){
        return &this->node_map[name];
    }

//================================================================
//                        Eigen Methods
//================================================================
/**
 * Methods for directly getting and setting Eigen::MatrixXd
*/

    Eigen::MatrixXd getGrad(std::string name){
        return this->node_map[name].getGrad();
    }
    Eigen::MatrixXd getValue(std::string name){
        return this->node_map[name].getValue();
    }
    Eigen::MatrixXd get(std::string name){
        return this->node_map[name].getValue();
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
    Eigen::MatrixXd getOutput(){
        return this->f->getValue();
    }

//================================================================
//                      Function Methods
//================================================================
/**
* Methods that are used on the last operator (f) 
*   without having to get f from the graph
*/
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
    Graph& backward(Eigen::MatrixXd partial_outer_derivative){
        this->f->backward(partial_outer_derivative);
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


nodeflow::Node* nodeflow::Graph::createOperator(Node* a, Node* b, std::string operator_name){
    //TODO: check for two-nodes operator if b is exist or not then throw error 
//================================================================
//                           Basic
//================================================================
    if (operator_name     == "add")          return new Add(a, b);
    else if (operator_name== "sub")          return new Subtract(a);
    else if (operator_name== "mul" 
        || operator_name  == "matmul" 
        || operator_name  == "dot" )         return new Mul(a, b); 
    else if (operator_name== "ew_mul")       return new EleWiseMul(a, b);
    else if (operator_name== "s_mul")        return new ScalarMul(a, b);
    else if (operator_name== "div")          return new Div(a, b);
    else if (operator_name== "ew_div")       return new EleWiseDiv(a, b);
    else if (operator_name== "s_div")        return new ScalarDiv(a, b);
    else if (operator_name== "pow")          return new Pow(a, b);
    else if (operator_name== "sqrt")         return new Sqrt(a);
    else if (operator_name== "invert")       return new Invert(a);
    else if (operator_name== "inverse")      return new Inverse(a);
//================================================================
//                        Trigonometry
//================================================================
    else if (operator_name== "sin")          return new Sin(a);
    else if (operator_name== "cos")          return new Cos(a);
    else if (operator_name== "tan")          return new Tan(a);
//================================================================
//                        Hyperbolic
//================================================================
    else if (operator_name== "sinh")         return new Sinh(a);
    else if (operator_name== "cosh")         return new Cosh(a);
    else if (operator_name== "tanh")         return new Tanh(a);
//================================================================
//                        Exponential
//================================================================
    else if (operator_name== "exp")          return new Exp(a);
//================================================================
//                        Logarithmic
//================================================================
    else if (operator_name== "ln")           return new Loge(a);
//================================================================
//                      Matrix Operators
//================================================================
    else if (operator_name== "sum")          return new Sum(a);
//================================================================
//                          Statistic
//================================================================
    else if (operator_name== "sigmoid")      return new Sigmoid(a);
    else if (operator_name== "relu")         return new ReLU(a);
    else if (operator_name== "leaky_relu")   return new LeakyReLU(a, b);
    else if (operator_name== "softmax")      return new Softmax(a);
    else throw std::invalid_argument("Operator " + operator_name + " is not found");
}
