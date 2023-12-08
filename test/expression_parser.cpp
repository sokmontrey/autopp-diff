#include <nodeflow/nodeflow.hpp>

using namespace nodeflow;

int main() {
  // std::cout << exParser("sin(cos((abc - #BCC) * $c + 1.5)) / -2*2") <<
  // std::endl;
  Graph f("sin(cos((#ew_mul(a_bc, def) + #BCC) * $c + 0. - 1.5)) / -2*2^5");
  // Graph f("1 / (c + -a) / (e+f) * sin(b)^2");

  return 0;
}
