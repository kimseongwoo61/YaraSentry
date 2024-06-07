#include <filesystem>
#include <iostream>
#include "file_spec.h"
#include "quarantine_manager.h"
#include "user_input.h"
#include "yara_handler.h"

using namespace std;

int main() {
    UserInput *user_input = new UserInput();
    YaraHandler *yara = new YaraHandler();
    FileSpec *file_info = new FileSpec();
    QuarantineManager *q_manager = new QuarantineManager();
    string scan_dir, vaccine_rule;

    // UI 정보 출력
    cout << " __    __                             ____                         __                        " << endl;
    cout << "/\\ \\  /\\ \\                           /\\  _`\\                      /\\ \\__                     " << endl;
    cout << "\\ `\\`\\\\/'/   __      _ __     __     \\ \\,\\L\\_\\      __     ___    \\ \\ ,_\\   _ __   __  __    " << endl;
    cout << " `\\ `\\ /'  /'__`\\   /\\`'__\\ /'__`\\    \\/_\\__ \\    /'__`\\ /' _ `\\   \\ \\ \\/  /\\`'__\\/\\ \\/\\ \\   " << endl;
    cout << "   `\\ \\ \\ /\\ \\L\\.\\_ \\ \\ \\/ /\\ \\L\\.\\_    /\\ \\L\\ \\ /\\  __/ /\\ \\/\\ \\   \\ \\ \\_ \\ \\ \\/ \\ \\ \\_\\ \\  " << endl;
    cout << "     \\ \\_\\\\ \\__/.\\_\\ \\ \\_\\ \\ \\__/.\\_\\   \\ `\\____\\\\ \\____\\\\ \\_\\ \\_\\   \\ \\__\\ \\ \\_\\  \\/`____ \\ " << endl;
    cout << "      \\/_/ \\/__/\\/_/  \\/_/  \\/__/\\/_/    \\/_____/ \\/____/ \\/_/\\/_/    \\/__/  \\/_/   `/___/> \\" << endl;
    cout << "                                                                                       /\\___/" << endl;
    cout << "                                                                                       \\/__/ " << endl;
    cout << "" << endl;
    cout << "                  -------------------------------------------------------------" << endl;
    cout << "                    YARA based AntiMalware - ver 1.0  |  Made by Kimseungwoo."   << endl;
    cout << "                  -------------------------------------------------------------" << endl;
    cout << "" << endl;
    cout << "" << endl;



    // 백신 룰 정보를 모두 컴파일한다.
    yara->setRulePath();

    // yara 라이브러리 초기화를 진행한다.
    yara->initialize();
    yara->compileRuleFile();


    // 검사 옵션을 입력받고 스캔을 진행한다.
    while (true)
    {   
        // 단일 파일 검사인 경우
        if (user_input->isSingleFileScan())
        {
            
            scan_dir = user_input->getDirectory();
            cout << "Start single file scan..." << endl;
            
            // 파일 스캔 시작
            if (!yara->scanFile(scan_dir))
            {
                std::cerr << "Failed to scan file." << std::endl;
                return 1;
            }

            // 스캔 결과 분석
            const auto& matchedRules = yara->getMatchedRules();
            if (matchedRules.size() > 0)
            {
              q_manager->logAction(q_manager->getCurrentTimeAsString());
              file_info->printFileSpec(scan_dir);
              q_manager->logAction("file directory : " + scan_dir);

              for (const auto& rule : matchedRules)
              {
                // 매칭된 항목 출력
                std::cout << "Matched Rule: " << rule
                          << " in file: " << scan_dir << std::endl;
                // 파일 정보와 매칭 룰을 로그 파일에 저장
                q_manager->logAction("-> " + rule + " [Matched]");
              }
              q_manager->quarantineFile(scan_dir);
              q_manager->deleteFile(scan_dir);
            }
        }

        else if (user_input->isRecursiveScan())
        {
            cout << "디렉터리 내부 파일을 모두 검사합니다. >>" << endl;

            // 검사할 디렉터리 경로 가져오기
            scan_dir = user_input->getDirectory();

            // 디렉터리 내부의 모든 파일 검사
            for (const auto& entry :
                std::filesystem::recursive_directory_iterator(scan_dir))
            {
                // 디렉터리가 아니라면 파일 스캔
                if (!entry.is_directory())
                {
                    // 파일 경로
                    std::string file_path = entry.path().string();
                    // 파일 스캔 시작
                    if (!yara->scanFile(file_path))
                    {
                        std::cerr << "Failed to scan file: " << file_path << std::endl;
                        return 1;
                    }
                    // 스캔 결과 분석
                    const auto& matchedRules = yara->getMatchedRules();

                    if (matchedRules.size() > 0)
                    {
                        q_manager->logAction(q_manager->getCurrentTimeAsString());
                        file_info->printFileSpec(file_path);
                        q_manager->logAction("file directory : " + file_path);

                        for (const auto& rule : matchedRules)
                        {
                            // 매칭된 항목 출력
                            std::cout << "Matched Rule: " << rule
                                    << " in file: " << file_path << std::endl;
                            // 파일 정보와 매칭 룰을 로그 파일에 저장
                            q_manager->logAction("-> " + rule + " [Matched]");
                        }
                        q_manager->quarantineFile(file_path);
                        q_manager->deleteFile(file_path);
                    }
                }
            }
        }


        std::string input;
        std::cout << "Do you want to quit this program? (y/n): ";
        std::getline(std::cin, input);

        if (input == "y" || input == "Y")
        {
            delete user_input;
            delete yara;
            delete file_info;
            delete q_manager;
            return 0;
        }
    }
}