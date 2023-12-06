#pragma once

namespace nodeflow {

std::string getOperatorName(std::string s);
std::string getOperatorArgs(std::string s);
std::pair<std::string, std::string> splitArgs(std::string s);
std::string reverseString(std::string s);
std::string removeSpaces(std::string s);

bool isNumber(std::string s);

std::string replaceMathConstants(std::string s);

} // namespace nodeflow

std::string nodeflow::getOperatorName(std::string s) {
  std::string temp = "";
  for (int i = 0; i < s.length(); i++) {
    if (s[i] == '(') {
      return temp;
    }
    temp += s[i];
  }
  return "";
}
std::string nodeflow::getOperatorArgs(std::string s) {
  std::string temp = "";
  bool bracket_found = false;
  int bracket_count = 0;

  for (int i = 0; i < s.length(); i++) {
    if (!bracket_found && s[i] == '(') {
      bracket_found = true;
      continue;
    } else if (bracket_found) {
      if (s[i] == '(') {
        bracket_count++;
      } else if (s[i] == ')') {
        if (bracket_count == 0) {
          return temp;
        }
        bracket_count--;
      }
      temp += s[i];
    }
  }
  return temp;
}
std::pair<std::string, std::string> nodeflow::splitArgs(std::string s) {
  // reverse for loop the string
  std::string first_args_temp = "";
  std::string second_args_temp = "";
  std::string temp;

  int bracket_count = 0;
  bool comma_found = false;

  for (int i = s.length() - 1; i >= 0; i--) {
    if (s[i] == ')') {
      bracket_count++;
    } else if (s[i] == '(') {
      bracket_count--;
    } else if (s[i] == ',' && bracket_count == 0 && !comma_found) {
      second_args_temp = temp;
      comma_found = true;
      temp = "";
      continue;
    }
    temp += s[i];
  }
  first_args_temp = temp;
  if (!comma_found)
    second_args_temp = "";

  std::string first_args = nodeflow::reverseString(first_args_temp);
  std::string second_args = nodeflow::reverseString(second_args_temp);

  return std::make_pair(first_args, second_args);
}

std::string nodeflow::reverseString(std::string s) {
  std::string temp;
  for (int i = s.length() - 1; i >= 0; i--) {
    temp += s[i];
  }
  return temp;
}
std::string nodeflow::removeSpaces(std::string s) {
  std::string temp = "";
  for (int i = 0; i < s.length(); i++) {
    if (s[i] != ' ') {
      temp += s[i];
    }
  }
  return temp;
}
bool nodeflow::isNumber(std::string s) {
  if (s.length() == 0)
    return false;

  for (int i = 0; i < s.length(); i++) {
    if (s[i] != '.' && s[i] != '-' && (s[i] < '0' || s[i] > '9') && s != "PI" &&
        s != "E") {
      return false;
    }
  }
  return true;
}

std::string nodeflow::replaceMathConstants(std::string s) {
  if (s == "PI")
    return "3.14159265358979323846";
  else if (s == "E")
    return "2.71828182845904523536";

  return s;
}
