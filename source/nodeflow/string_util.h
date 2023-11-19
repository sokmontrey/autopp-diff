#pragma once

#include <string>
#include <iostream>

namespace nodeflow {

std::string reverseString(std::string s){
    std::string temp;
    for(int i = s.length() - 1; i >= 0; i--){
        temp += s[i];
    }
    return temp;
}
std::string removeSpaces(std::string s){
    std::string temp="";
    for(int i = 0; i < s.length(); i++){
        if (s[i] != ' '){
            temp += s[i];
        }
    }
    return temp;
}
bool isNumber(std::string s){
    if (s.length() == 0) return false;

    for(int i = 0; i < s.length(); i++){
        if (s[i] != '.' && s[i] != '-' && (s[i] < '0' || s[i] > '9')){
            return false;
        }
    }
    return true;
}

} //namespace nodeflow