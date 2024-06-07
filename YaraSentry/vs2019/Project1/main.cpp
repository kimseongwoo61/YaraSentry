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

    // UI ���� ���
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



    // ��� �� ������ ��� �������Ѵ�.
    yara->setRulePath();

    // yara ���̺귯�� �ʱ�ȭ�� �����Ѵ�.
    yara->initialize();
    yara->compileRuleFile();


    // �˻� �ɼ��� �Է¹ް� ��ĵ�� �����Ѵ�.
    while (true)
    {   
        // ���� ���� �˻��� ���
        if (user_input->isSingleFileScan())
        {
            
            scan_dir = user_input->getDirectory();
            cout << "Start single file scan..." << endl;
            
            // ���� ��ĵ ����
            if (!yara->scanFile(scan_dir))
            {
                std::cerr << "Failed to scan file." << std::endl;
                return 1;
            }

            // ��ĵ ��� �м�
            const auto& matchedRules = yara->getMatchedRules();
            if (matchedRules.size() > 0)
            {
              q_manager->logAction(q_manager->getCurrentTimeAsString());
              file_info->printFileSpec(scan_dir);
              q_manager->logAction("file directory : " + scan_dir);

              for (const auto& rule : matchedRules)
              {
                // ��Ī�� �׸� ���
                std::cout << "Matched Rule: " << rule
                          << " in file: " << scan_dir << std::endl;
                // ���� ������ ��Ī ���� �α� ���Ͽ� ����
                q_manager->logAction("-> " + rule + " [Matched]");
              }
              q_manager->quarantineFile(scan_dir);
              q_manager->deleteFile(scan_dir);
            }
        }

        else if (user_input->isRecursiveScan())
        {
            cout << "���͸� ���� ������ ��� �˻��մϴ�. >>" << endl;

            // �˻��� ���͸� ��� ��������
            scan_dir = user_input->getDirectory();

            // ���͸� ������ ��� ���� �˻�
            for (const auto& entry :
                std::filesystem::recursive_directory_iterator(scan_dir))
            {
                // ���͸��� �ƴ϶�� ���� ��ĵ
                if (!entry.is_directory())
                {
                    // ���� ���
                    std::string file_path = entry.path().string();
                    // ���� ��ĵ ����
                    if (!yara->scanFile(file_path))
                    {
                        std::cerr << "Failed to scan file: " << file_path << std::endl;
                        return 1;
                    }
                    // ��ĵ ��� �м�
                    const auto& matchedRules = yara->getMatchedRules();

                    if (matchedRules.size() > 0)
                    {
                        q_manager->logAction(q_manager->getCurrentTimeAsString());
                        file_info->printFileSpec(file_path);
                        q_manager->logAction("file directory : " + file_path);

                        for (const auto& rule : matchedRules)
                        {
                            // ��Ī�� �׸� ���
                            std::cout << "Matched Rule: " << rule
                                    << " in file: " << file_path << std::endl;
                            // ���� ������ ��Ī ���� �α� ���Ͽ� ����
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