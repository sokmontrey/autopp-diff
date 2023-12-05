#include <nodeflow/nodeflow.h>

using namespace nodeflow;

int main(){
    // std::cout << exParser("sin(cos((abc - #BCC) * $c + 1.5)) / -2*2") << std::endl;
    Graph f("sin(cos((ew_mul(a_bc, def) - #BCC) * $c + 1.5)) / -2*2^5");
    
    return 0;
}
