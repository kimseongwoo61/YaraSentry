// yara_handler.cpp
#include "yara_handler.h"
#include <iostream>
#include <filesystem>

// yara  룰 검사를 위한 컴파일 객체 포인터와 룰 디렉터리 정보를 
// null로 초기화한다. 
YaraHandler::YaraHandler()
{
    compiled_rules = nullptr;
}

// yara 컴파일 객체를 해제하고 yara 라이브러리를 해제한다.
YaraHandler::~YaraHandler()
{
  if (compiled_rules)
      yr_rules_destroy(compiled_rules);

  yr_finalize();
}

// yara 라이브러리를 초기화 한다.
// 만약 에러가 발생하면 에러 정보를 출력 후 종료한다.
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

// yara 룰 경로 디렉터리를 설정한다.
void YaraHandler::setRulePath() {
    string path;
    std::cout << "Input your .yar rule path: ";
    std::getline(std::cin, path);

    YaraHandler::yara_rule_path = path;
}

// yara 룰을 모두 컴파일한다.
// 만약 하나라도 에러가 발생하면 에러 정보 출력 후 프로그램을 종료한다.
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

// 컴파일된 yara 룰 정보를 기반으로 파일을 스캔한다.
// 스캔 결과는 콜백 함수에 저장된다.
bool YaraHandler::scanFile(const std::string& filePath)
{
  if (!compiled_rules)
  {
    std::cerr << "YARA rules not compiled. Call compileRuleFile() first."
              << std::endl;
    return false;
  }

  matched_rules.clear();  // 스캔 결과를 저장하기 전에 초기화

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

// .yar 확장자의 탐지 룰 파일 절대 경로를 컨테이너로 반환한다.
vector<string> YaraHandler::findYaraFiles(
    const std::string& directory)
{
    namespace fs = std::filesystem;
  std::vector<std::string> yaraFiles;

  // 디렉토리 내의 모든 파일을 검색
  for (const auto& entry : fs::directory_iterator(directory))
  {
    // 파일인 경우 확장자를 확인하여 ".yar" 파일인지 판별
    if (entry.is_regular_file() && entry.path().extension() == ".yar")
    {
      yaraFiles.push_back(entry.path().string());
    }
  }

  return yaraFiles;
}

// 스캔 결과 매칭된 룰 정보를 가져온다.
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
    // 추가적인 콜백 메시지 처리
    // ...
  }
  return CALLBACK_CONTINUE;
}

// 정적 래퍼 함수
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
