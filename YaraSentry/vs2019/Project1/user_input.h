#pragma once

#ifndef USER_INPUT_H
#define USER_INPUT_H

#include <string>

class UserInput
{
 public:
  std::string getDirectory();
  bool isSingleFileScan();
  bool isRecursiveScan();
};

#endif  // USER_INPUT_H
