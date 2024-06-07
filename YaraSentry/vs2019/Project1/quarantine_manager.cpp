// quarantine_manager.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "quarantine_manager.h"
#include <filesystem>
#include <fstream>
#include <iostream>

bool QuarantineManager::quarantineFile(const std::string& filePath)
{
  std::string quarantinePath =
      "quarantine/" + std::filesystem::path(filePath).filename().string() +
      ".quarantine";
  std::filesystem::create_directories("quarantine");
  return std::filesystem::copy_file(filePath, quarantinePath);
}

bool QuarantineManager::deleteFile(const std::string& filePath)
{
  return std::filesystem::remove(filePath);
}

void QuarantineManager::logAction(const std::string& message)
{
  std::ofstream logFile("./quarantine.log", std::ios_base::app);
  logFile << message << std::endl;
}

std::string QuarantineManager::getCurrentTimeAsString() {
  // 현재 시간 구하기
  std::time_t currentTime = std::time(nullptr);

  // 현재 시간을 문자열로 변환
  char buffer[80];
  std::strftime(
      buffer,
      sizeof(buffer),
      "%Y-%m-%d %H:%M:%S",
      std::localtime(&currentTime));

  // 문자열로 변환된 현재 시간 반환
  return std::string(buffer);
}
