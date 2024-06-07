// quarantine_manager.h
#ifndef QUARANTINE_MANAGER_H
#define QUARANTINE_MANAGER_H

#include <string>
#include <iostream>
#include <ctime>
#include <string>
class QuarantineManager
{
 public:
  bool quarantineFile(const std::string& filePath);
  bool deleteFile(const std::string& filePath);
  void logAction(const std::string& message);
  std::string getCurrentTimeAsString();
};

#endif  // QUARANTINE_MANAGER_H
