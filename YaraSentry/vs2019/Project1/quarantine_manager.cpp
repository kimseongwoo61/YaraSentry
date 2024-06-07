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
  // ���� �ð� ���ϱ�
  std::time_t currentTime = std::time(nullptr);

  // ���� �ð��� ���ڿ��� ��ȯ
  char buffer[80];
  std::strftime(
      buffer,
      sizeof(buffer),
      "%Y-%m-%d %H:%M:%S",
      std::localtime(&currentTime));

  // ���ڿ��� ��ȯ�� ���� �ð� ��ȯ
  return std::string(buffer);
}
