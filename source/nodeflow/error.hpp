#pragma once

namespace nodeflow {
namespace error {

void report(std::string where, std::string msg, std::string str, int pos) {
  std::cerr << "Error: " << where << ": " << msg << "\n\"" << str << "\"\n"
            << std::string(pos, ' ') << "^" << std::endl;
  exit(1);
}

} // namespace error
} // namespace nodeflow
