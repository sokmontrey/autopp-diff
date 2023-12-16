#include <nodeflow/nodeflow.hpp>

using namespace nodeflow;

int main() {
  // Graph f("abc", "BCC", "c");
  Graph f("a + b * c", {{"a", Node::Scalar(1.0)},
                        {"b", Node::Scalar(2.0)},
                        {"c", Node::Scalar(3.0)}});

  return 0;
}
