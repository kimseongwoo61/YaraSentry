// yara_handler.cpp
#include "yara_handler.h"
#include <iostream>
#include <filesystem>

// yara  �� �˻縦 ���� ������ ��ü �����Ϳ� �� ���͸� ������ 
// null�� �ʱ�ȭ�Ѵ�. 
YaraHandler::YaraHandler()
{
    compiled_rules = nullptr;
}

// yara ������ ��ü�� �����ϰ� yara ���̺귯���� �����Ѵ�.
YaraHandler::~YaraHandler()
{
  if (compiled_rules)
      yr_rules_destroy(compiled_rules);

  yr_finalize();
}

// yara ���̺귯���� �ʱ�ȭ �Ѵ�.
// ���� ������ �߻��ϸ� ���� ������ ��� �� �����Ѵ�.
bool YaraHandler::initialize()
{
  int result = yr_initialize();
  if (result != ERROR_SUCCESS)
  {
    std::cerr << "YARA initialization failed: " << result << std::endl;
    return false;
  }
  return true;
}

// yara �� ��� ���͸��� �����Ѵ�.
void YaraHandler::setRulePath() {
    string path;
    std::cout << "Input your .yar rule path: ";
    std::getline(std::cin, path);

    YaraHandler::yara_rule_path = path;
}

// yara ���� ��� �������Ѵ�.
// ���� �ϳ��� ������ �߻��ϸ� ���� ���� ��� �� ���α׷��� �����Ѵ�.
bool YaraHandler::compileRuleFile()
{
  YR_COMPILER* compiler = nullptr;
  std::vector<std::string> yaraFiles = YaraHandler::findYaraFiles(YaraHandler::yara_rule_path);
  int result = yr_compiler_create(&compiler);

  
  if (result != ERROR_SUCCESS)
  {
    std::cerr << "Failed to create YARA compiler: " << result << std::endl;
    return false;
  }

  FILE* ruleFile;

  for (const auto& path : yaraFiles)
  {
    if (fopen_s(&ruleFile, path.c_str(), "r") != 0)
    {
      std::cerr << "Failed to open YARA rule file: " << path
                << std::endl;
      yr_compiler_destroy(compiler);
      return false;
    }

    int errors = yr_compiler_add_file(
        compiler, ruleFile, nullptr, path.c_str());
    fclose(ruleFile);

    if (errors > 0)
    {
      std::cerr << "Failed to compile YARA rule file: " << path
                << std::endl;
      yr_compiler_destroy(compiler);
      return false;
    }

  }

  result = yr_compiler_get_rules(compiler, &compiled_rules);
  yr_compiler_destroy(compiler);

  if (result != ERROR_SUCCESS)
  {
    std::cerr << "Failed to get compiled rules: " << result << std::endl;
    return false;
  }

  return true;
}

// �����ϵ� yara �� ������ ������� ������ ��ĵ�Ѵ�.
// ��ĵ ����� �ݹ� �Լ��� ����ȴ�.
bool YaraHandler::scanFile(const std::string& filePath)
{
  if (!compiled_rules)
  {
    std::cerr << "YARA rules not compiled. Call compileRuleFile() first."
              << std::endl;
    return false;
  }

  matched_rules.clear();  // ��ĵ ����� �����ϱ� ���� �ʱ�ȭ

  int result = yr_rules_scan_file(
      compiled_rules,
      filePath.c_str(),
      0,
      YaraHandler::yaraCallbackFunctionWrapper,
      this,
      0);

  if (result != ERROR_SUCCESS)
  {
    std::cerr << "YARA scan failed for file: " << filePath
              << " with error code: " << result << std::endl;
    return false;
  }

  return true;
}

// .yar Ȯ������ Ž�� �� ���� ���� ��θ� �����̳ʷ� ��ȯ�Ѵ�.
vector<string> YaraHandler::findYaraFiles(
    const std::string& directory)
{
    namespace fs = std::filesystem;
  std::vector<std::string> yaraFiles;

  // ���丮 ���� ��� ������ �˻�
  for (const auto& entry : fs::directory_iterator(directory))
  {
    // ������ ��� Ȯ���ڸ� Ȯ���Ͽ� ".yar" �������� �Ǻ�
    if (entry.is_regular_file() && entry.path().extension() == ".yar")
    {
      yaraFiles.push_back(entry.path().string());
    }
  }

  return yaraFiles;
}

// ��ĵ ��� ��Ī�� �� ������ �����´�.
const vector<string>& YaraHandler::getMatchedRules() const
{
  return matched_rules;
}



int YaraHandler::yaraCallbackFunction(
    YR_SCAN_CONTEXT* context,
    int message,
    void* message_data)
{
  switch (message)
  {
  case CALLBACK_MSG_RULE_MATCHING:
  {
    YR_RULE* rule = (YR_RULE*) message_data;
    std::string ruleName = rule->identifier;
    //std::cout << "Matched rule: " << ruleName << std::endl;
    YaraHandler::matched_rules.push_back(ruleName);
    break;
  }
  case CALLBACK_MSG_RULE_NOT_MATCHING:
  {
    YR_RULE* rule = (YR_RULE*) message_data;
    std::string ruleName = rule->identifier;
    //std::cout << "Not matched rule: " << ruleName << std::endl;
    break;
  }
    // �߰����� �ݹ� �޽��� ó��
    // ...
  }
  return CALLBACK_CONTINUE;
}

// ���� ���� �Լ�
int YaraHandler::yaraCallbackFunctionWrapper(
    YR_SCAN_CONTEXT* context,
    int message,
    void* message_data,
    void* user_data)
{
  YaraHandler* handler = static_cast<YaraHandler*>(user_data);
  return handler->yaraCallbackFunction(
      context, message, message_data);
}
