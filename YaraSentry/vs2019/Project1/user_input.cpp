#include "user_input.h"
#include <iostream>
#include <string>

std::string UserInput::getDirectory()
{
  std::string path;
  std::cout << "Enter the directory to scan: ";
  std::getline(std::cin, path);
  return path;
}

bool UserInput::isSingleFileScan()
{
  std::string input;
  std::cout << "Do you want to scan a single file in the directory? (y/n): ";
  std::getline(std::cin, input);
  return (input == "y" || input == "Y");
}

bool UserInput::isRecursiveScan()
{
  std::string input;
  std::cout << "Do you want to scan all subdirectories recursively? (y/n): ";
  std::getline(std::cin, input);
  return (input == "y" || input == "Y");
}
