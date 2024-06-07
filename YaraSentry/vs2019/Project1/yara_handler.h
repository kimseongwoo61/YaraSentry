#ifndef YARA_HANDLER_H
#define YARA_HANDLER_H

#include <yara.h>
#include <string>
#include <vector>


using namespace std;

class YaraHandler
{
 public:
  YaraHandler();
  ~YaraHandler();

  bool initialize();
  void setRulePath();
  bool compileRuleFile();
  bool scanFile(const std::string& filePath);
  const std::vector<std::string>& getMatchedRules() const;
  std::vector<std::string> findYaraFiles(const std::string& directory);

 private:
  vector<string> matched_rules;
  string yara_rule_path;
  YR_RULES* compiled_rules;


  static int yaraCallbackFunctionWrapper(
      YR_SCAN_CONTEXT* context,
      int message,
      void* message_data,
      void* user_data);
  int yaraCallbackFunction(
      YR_SCAN_CONTEXT* context,
      int message,
      void* message_data);
};

#endif  // YARA_HANDLER_H
