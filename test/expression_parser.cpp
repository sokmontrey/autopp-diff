#include <nodeflow/nodeflow.hpp>

using namespace nodeflow;

int main() {
  Graph f("a", "b", "c");

  f.define("a", Node::Scalar(1));
  f.define("b", Node::Scalar(2));
  f.define("c", Node::Scalar(3));

  f = "a + b + c + 9";

  f().print();

  f.gradient();

  std::cout << f.partial("a") << std::endl;

  // f = "sin(cos((abc - #BCC) * $c + 1.5)) / -2*2";
  // std::endl;
  // Graph f("sin(cos((#ew_mul(a_bc, def) + #BCC) * $c + 0. - 1.5)) /
  // -2*2^5"); Graph f("1 / (c + -a) / (e+f) * sin(b)^2");
  // Graph::forward(f);
  // Graph::backward(f);
  // Graph::clear(f);
  // auto varNodes = Graph::getVariableNodes(f);

  return 0;
}
