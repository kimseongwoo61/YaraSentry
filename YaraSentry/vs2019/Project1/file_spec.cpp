#include "file_spec.h"
#include <filesystem>
#include <iostream>

void FileSpec::printFileSpec(const std::string& filePath)
{
  std::filesystem::path path(filePath);
  if (std::filesystem::exists(path))
  {
    std::cout << "File Name: " << path.filename() << std::endl;
    std::cout << "File Path: " << path.string() << std::endl;
    std::cout << "File Size: " << std::filesystem::file_size(path) << " bytes"
              << std::endl;
    // �߰����� PE ������ �ʿ信 ���� �����մϴ�.
  }
  else
  {
    std::cerr << "File does not exist: " << filePath << std::endl;
  }
}
