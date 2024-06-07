# YaraSentry: Simple Antivirus Program

## Project Overview

YaraSentry is a simple antivirus program built on the YARA library. It enables fast file inspection using detection rules provided by YARA.

---

### Supported Environments

- **Operating Systems**: Windows 10, Windows 8.1, Windows 7 (64-bit)
- **Development Environment**: Visual Studio 2019 or higher
- **Dependencies**: YARA library

---

### Key Features

1. **File Inspection**
    
    Inspects file patterns using pre-written YARA rule information.
    
2. **Quarantine Facility**
    
    Files matching patterns are renamed with the extension ".quarantine" and moved to a quarantine folder, while the original files are deleted.
    
3. **Logging**
    
    Records inspection time, pattern information, and path details for matched items in logs.
    

---

### Installation

1. Clone the YARA project into a separate directory.
    
    ```bash
    git clone https://github.com/VirusTotal/yara.git
    ```
    
2. Open Visual Studio and build the YARA project.
3. Clone the YaraSentry repository.
    
    ```bash
    git clone https://github.com/kimseongwoo61/YaraSentry.git
    ```
    
4. Configure dependencies for YaraSentry and build the project.
    - Additional Include Directories: `{yara_project}\libyara\include`
    - Additional Library Directories: `{yara_project}\windows\{your_vs_version_folder}\libyara\$(Configuration)`
    - Additional Dependencies: `ws2_32.lib`, `crypt32.lib`, `libyara$(PlatformArchitecture).lib`

---

### Usage

1. Input YARA rule path.    
    ![https://prod-files-secure.s3.us-west-2.amazonaws.com/13387632-ff1f-4d43-99b8-5781a6fd4412/16b284b4-3b0a-46d5-9fad-893693abe76e/noname01.bmp](https://github.com/kimseongwoo61/YaraSentry/blob/main/screenshot/1.bmp)
    
2. Specify whether to perform single file or recursive directory scans.    
    ![https://prod-files-secure.s3.us-west-2.amazonaws.com/13387632-ff1f-4d43-99b8-5781a6fd4412/ffa2928c-f7e5-4ebd-97d5-f3eba6f33889/7bcd92c2-dc56-4846-b3ef-7053605828a1.png](https://github.com/kimseongwoo61/YaraSentry/blob/main/screenshot/2.png)
    
3. View quarantine and logging information.
    ![https://prod-files-secure.s3.us-west-2.amazonaws.com/13387632-ff1f-4d43-99b8-5781a6fd4412/452bcfeb-a7e9-4e01-9b50-5d05cfb47f87/adsf.bmp](https://github.com/kimseongwoo61/YaraSentry/blob/main/screenshot/3.bmp)
    
    ![https://prod-files-secure.s3.us-west-2.amazonaws.com/13387632-ff1f-4d43-99b8-5781a6fd4412/891e3de0-7ec1-4567-a870-589313be9ce7/noname01.bmp](https://github.com/kimseongwoo61/YaraSentry/blob/main/screenshot/4.bmp)
    

---

## References

For program modifications or detailed analysis of its functionality, please refer to the code documentation below.
- [YaraSentry_Documents](https://github.com/kimseongwoo61/YaraSentry/blob/main/YaraSentry%20Documents.MD)
