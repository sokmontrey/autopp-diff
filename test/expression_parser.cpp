#include <nodeflow/nodeflow.hpp>

using namespace nodeflow;

int main() {
  ExScanner scanner("sin(cos((abc - #BCC) * $c + 1.5)) / -2*2");
  scanner.scan();
  scanner.printTokens();

  ExParser parser(scanner.getTokens());
  parser.parse()->print();

  // std::endl;
  // Graph f("sin(cos((#ew_mul(a_bc, def) + #BCC) * $c + 0. - 1.5)) /
  // -2*2^5"); Graph f("1 / (c + -a) / (e+f) * sin(b)^2");
  // Graph::forward(f);
  // Graph::backward(f);
  // Graph::clear(f);
  // auto varNodes = Graph::getVariableNodes(f);

  return 0;
}
