#include <nodeflow/nodeflow.hpp>

using namespace nodeflow;

int main() {
  Graph f("abc", "BCC", "c");

  f = "sin(cos((abc - #BCC) * c + 1.5)) / -2*2";

  return 0;
}
