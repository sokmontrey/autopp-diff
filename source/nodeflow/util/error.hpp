#pragma once

#include <iostream>
#include <string>

namespace nodeflow {
namespace error {

inline 
void report(std::string where, std::string msg, std::string str, int pos){
  std::cerr << "Error: " << where << ": " << msg << "\n\"" << str << "\"\n"
            << std::string(pos, ' ') << "^" << std::endl;
  exit(1);
}

} // namespace error
} // namespace nodeflow
