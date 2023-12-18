#include <nodeflow/nodeflow.hpp>

using namespace nodeflow;

int main() {
  Graph f("relu(w * x + b)", {{"w", Node::Random(3, 2)},
                              {"x", Node::Vector(2)},
                              {"b", Node::Random(3)}});

  Graph e("(t - $y)^2 / 2", {{"t", Node::Vector(3)}}, {{"y", f}});

  f.node("x")->setVector({1, 2});
  print(e.evaluate());

  e.gradient();
  f.node("w")->setMatrix(f.partial("w") * 0.1);
  f.node("b")->setVector(f.partial("b") * 0.1);

  e.destroy();

  return 0;
}
