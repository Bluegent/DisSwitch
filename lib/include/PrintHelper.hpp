#pragma once
#include <string>
#include <iostream>

template <typename T>
void printItem(const std::string& name, const T& value, bool newLine = true)
{
    std::cout << '\t' << name << " : " << value << (newLine ? '\n' : ' ');
}

