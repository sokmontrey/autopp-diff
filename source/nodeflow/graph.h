#pragma once 

namespace nodeflow {

/**
 * High level class for creating a graph of nodes
 * params:
 *     s: string of math expression with composition of functions
 *     node_map: map of nodes that are used in the graph
 *     subgraphs: map of subgraphs that are used in the graph
 *
 * Expression rules:
 *      - Function names are in lower case
 *      - Argument can be variable, constant, or subgraph
 *          - Variable: any string that is not a number or a subgraphs
 *              - Example: "a", "b", "_a_1", "hello"
 *          - Constant: when an argument is a number or a math constant, it is considered as a constant
 *              - Partial Derivative will not be calculated for constants
 *              - Example: "1", "3.14", "PI", "E"
 *          - Subgraph: when an argument name starts with "$", it is considered as a subgraph
 *              - Example: "$subgraph_1", "$subgraph_2"
 *              - A subgraph MUST be a reference to Node or to an OperatorNode. 
 *              - The reference MUST be provided in the constructor of the main graph
 *                  - Example 1: 
 *
 *                      Node a; 
 *                      Graph g("sin($a)", {
 *                          {"$a", &a} // map for subgraphs
 *                      });
 *
 *                 - Example 2:
 *
 *                      //... define nodes a and b
 *                      Add f(&a, &b);
 *                      Graph g("sin($f)", {
 *                          {"$f", &f} // map for subgraphs
 *                      });
 *
 *
 *                  - Example 3:
 *
 *                      //... define Graph f
 *                      Graph g("sin($f)", {
 *                          // .getEnd() or .getF() can be used to get the last operator node of a graph
 *                          {"$f", f.getEnd()} 
 *                      }); 
 */

class Graph{
private:
    std::map<std::string, Node> node_map;
    std::vector<Node*> operator_nodes;
    std::map<std::string, Node*> sub_graphs;
    Node* f = nullptr;
    std::string expression;

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
    Node* getSubgraph(std::string node_name){
        if (this->sub_graphs.find(node_name) == this->sub_graphs.end()){
            throw std::runtime_error("Subgraph " + node_name + " is not defined");
        }
        return this->sub_graphs[node_name];
    }
    Node* createNode(std::string node_name){
        if (node_name[0] == '#' && node_name[1] == '$') {
            //a constant subgraph
            getSubgraph(node_name)->constant();
            return getSubgraph(node_name);
        } else if (node_name[0] == '#') {
            //a constant
            if (this->node_map.find(node_name) == this->node_map.end()){
                this->node_map[node_name] = Node();
            }
            this->node_map[node_name].constant();
            return &this->node_map[node_name];
        } else if (node_name[0] == '$') {
            //a subgraph
            return getSubgraph(node_name);
        } else {
            //a variable node or a constant node
            bool is_number = isNumber(node_name);
            std::string s = (is_number ? "#" : "") + node_name;

            if(this->node_map.find(s) == this->node_map.end()){
                //if node is not already exist
                this->node_map[s] = is_number 
                    ? Node::ScalarConst(std::stod(replaceMathConstants(node_name))) 
                    : Node();
            }
            return &this->node_map[s];
        }   
    }
    Node* createOperator(Node* a, Node* b, std::string operator_name);

//================================================================
//                         Initializer
//================================================================

    void init(){
        this->operator_nodes.clear();
        // this->f = parse(removeSpaces(this->expression));
        // this->f->finished();
        if (this->expression == "") error::report("Graph", "Expression is empty!", this->expression, 0);
        Scanner scanner(this->expression);
        scanner.scan();
        scanner.printTokens();
    }

public:

//================================================================
//                  No sub_graphs constructor
//================================================================
    
    Graph(std::string s){
        this->expression = s;
        init();
    }
    Graph(
        std::string s, 
        std::map<std::string, Node> node_map
    ){
        this->expression = s;
        this->node_map = node_map;
        init();
    }

//================================================================
//                          No node_map
//================================================================

    //pointer to subgraphs (end node)
    Graph(
        std::string s, 
        std::map<std::string, Node*> subgraphs
    ){
        this->expression = s;
        this->sub_graphs = subgraphs;
        init();
    }
    //pointer to subgraphs
    Graph(
        std::string s, 
        std::map<std::string, Graph*> subgraphs
    ){
        this->expression = s;
        for (auto it = subgraphs.begin(); it != subgraphs.end(); it++){
            this->sub_graphs[it->first] = it->second->getEnd();
        }
        init();
    }
    //reference to subgraphs
    Graph(
        std::string s, 
        std::map<std::string, Graph&> subgraphs
    ){
        this->expression = s;
        for (auto it = subgraphs.begin(); it != subgraphs.end(); it++){
            this->sub_graphs[it->first] = it->second.getEnd();
        }
        init();
    }

//================================================================
//                  node_map with subgraphs
//================================================================
    
    //pointer to subgraphs (end node)
    Graph(
        std::string s, 
        std::map<std::string, Node> node_map, 
        std::map<std::string, Node*> subgraphs
    ){
        this->expression = s;
        this->node_map = node_map;
        this->sub_graphs = subgraphs;
        init();
    }
    //pointer to subgraphs
    Graph(
        std::string s, 
        std::map<std::string, Node> node_map, 
        std::map<std::string, Graph*> subgraphs
    ){
        this->expression = s;
        this->node_map = node_map;
        for (auto it = subgraphs.begin(); it != subgraphs.end(); it++){
            this->sub_graphs[it->first] = it->second->getEnd();
        }
        init();
    }
    //reference to subgraphs
    Graph(
        std::string s, 
        std::map<std::string, Node> node_map, 
        std::map<std::string, Graph&> subgraphs
    ){
        this->expression = s;
        this->node_map = node_map;
        for (auto it = subgraphs.begin(); it != subgraphs.end(); it++){
            this->sub_graphs[it->first] = it->second.getEnd();
        }
        init();
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

    std::vector<std::string> getVariableName() {
        std::vector<std::string> variable_names;
        for (auto it = this->node_map.begin(); it != this->node_map.end(); it++){
            if (!it->second.isConstant()){
                variable_names.push_back(it->first);
            }
        }
        return variable_names;
    }

    std::vector<Node*> getVariableNode() {
        std::vector<Node*> variable_nodes;
        for (auto it = this->node_map.begin(); it != this->node_map.end(); it++){
            if (!it->second.isConstant()){
                variable_nodes.push_back(&it->second);
            }
        }
        return variable_nodes;
    }

    std::vector<Node*> getAllLeaveNode(){
        return this->f->getAllLeaveNode();
    }

    std::vector<Node*> getAllVariableNode(){
        std::vector<Node*> all_nodes = this->getAllLeaveNode();
        for (int i = 0; i < all_nodes.size(); i++){
            if (all_nodes[i]->isConstant()){
                all_nodes.erase(all_nodes.begin() + i);
                i--;
            }
        }
        return all_nodes;
    }

    Node* getOperatorNode(int i){
        return this->operator_nodes[i];
    }
    size_t getOperatorLength(){
        return this->operator_nodes.size();
    }

//================================================================
//                        Eigen Methods
//================================================================
/**
 * Methods for directly getting and setting Eigen::MatrixXd
*/

    /* getter */
    Eigen::MatrixXd getGrad(std::string name){
        return this->node_map[name].getGrad();
    }

    double getValue(std::string name, size_t i, size_t j){
        return this->node_map[name].getValue(i, j);
    }
    double getValue(std::string name){
        return this->node_map[name].getValue(0, 0);
    }
    double getValue(){
        return this->f->getValue(0, 0);
    }

    Eigen::MatrixXd getMatrix(std::string name){
        return this->node_map[name].getMatrix();
    }
    Eigen::MatrixXd getMatrix(){
        return this->f->getMatrix();
    }

    /* end node getter */
    Node* getF(){
        return this->f;
    }
    Node* getEnd(){
        return this->f;
    }

    /* setter */
    Graph& setValue(std::string name, size_t i, size_t j, double value){
        this->node_map[name].setValue(i, j, value);
        return *this;
    }
    Graph& setValue(std::string name, double value){
        this->node_map[name].setValue(0,0,value);
        return *this;
    }
    Graph& setMatrix(std::string name, Eigen::MatrixXd value){
        this->node_map[name].setMatrix(value);
        return *this;
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
        this->f->reset();
        this->f->forward();
        return *this;
    }
    Eigen::MatrixXd operator()(){
        return this->f->forward();
    }
    Graph& backward(){
        this->f->reset();
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
    //check if operator is a constant (start with #)
    if (operator_name[0] == '#') {
        nodeflow::Node* n = nodeflow::Graph::createOperator(a, b, operator_name.substr(1));
        n->constant();
        return n;
    }
//================================================================
//                           Basic
//================================================================
    if (operator_name     == "add")          return new Add         (a, b);
    else if (operator_name== "sub"
        || operator_name  == "subtract")     return new Subtract    (a, b);
    else if (operator_name== "mul" 
        || operator_name  == "matmul" 
        || operator_name  == "dot" )         return new Mul         (a, b); 
    else if (operator_name== "ew_mul")       return new EleWiseMul  (a, b);
    else if (operator_name== "s_mul")        return new ScalarMul   (a, b);
    else if (operator_name== "div")          return new Div         (a, b);
    else if (operator_name== "ew_div")       return new EleWiseDiv  (a, b);
    else if (operator_name== "s_div")        return new ScalarDiv   (a, b);
    else if (operator_name== "pow")          return new Pow         (a, b);
    else if (operator_name== "sqrt")         return new Sqrt        (a);
    else if (operator_name== "invert")       return new Invert      (a);
    else if (operator_name== "inverse")      return new Inverse     (a);
//================================================================
//                        Trigonometry
//================================================================
    else if (operator_name== "sin")          return new Sin         (a);
    else if (operator_name== "cos")          return new Cos         (a);
    else if (operator_name== "tan")          return new Tan         (a);
//================================================================
//                        Hyperbolic
//================================================================
    else if (operator_name== "sinh")         return new Sinh        (a);
    else if (operator_name== "cosh")         return new Cosh        (a);
    else if (operator_name== "tanh")         return new Tanh        (a);
//================================================================
//                        Exponential
//================================================================
    else if (operator_name== "exp")          return new Exp         (a);
//================================================================
//                        Logarithmic
//================================================================
    else if (operator_name== "ln")           return new Loge        (a);
//================================================================
//                      Matrix Operators
//================================================================
    else if (operator_name== "sum")          return new Sum         (a);
//================================================================
//                          Statistic
//================================================================
    else if (operator_name== "sigmoid")      return new Sigmoid     (a);
    else if (operator_name== "relu")         return new ReLU        (a);
    else if (operator_name== "leaky_relu")   return new LeakyReLU   (a, b);
    else if (operator_name== "softmax")      return new Softmax     (a);
    else throw std::invalid_argument("Operator " + operator_name + " is not found");
}
