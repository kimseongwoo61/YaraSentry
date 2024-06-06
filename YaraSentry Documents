# YaraSentry Documents

## 1. Class Information

---

### 1) FileSpec Class

- **Primary Functionality:** Display information about the inspected file, including its name, path, and size.
- **Source Code:** `file_spec.cpp`, `file_spec.h`
- **Methods:**
    - `printFileSpec(const std::string& filePath)`: Prints the name, path, and size information of the given file.

### 2) QuarantineManager Class

- **Primary Functionality:** Delete or move files to a quarantine directory, and log the corresponding actions.
- **Source Code:** `quarantine_manager.cpp`, `quarantine_manager.h`
- **Methods:**
    - `quarantineFile(const std::string& filePath)`: Creates a "quarantine" folder in the virus execution path, renames the file to "filename.quarantine", and copies it to the quarantine path.
    - `deleteFile(const std::string& filePath)`: Deletes the specified file.
    - `logAction(const std::string& message)`: Saves the provided message to "quarantine.log" in the execution file path. The content is appended.
    - `getCurrentTimeAsString()`: Returns the current time information in the "%Y-%m-%d %H:%M:%S" format.

### 3) UserInput Class

- **Primary Functionality:** Handle user input for the inspection directory path and options.
- **Source Code:** `user_input.cpp`, `user_input.h`
- **Methods:**
    - `getDirectory()`: Receives the inspection file path information.
    - `isSingleFileScan()`: Determines whether it is a single file scan.
    - `isRecursiveScan()`: Determines whether to recursively scan subdirectories.

### 4) YaraHandler Class

- **Primary Functionality:** Initialize the YARA library, compile YARA rule files, scan files for matching rule information.
- **Source Code:** `yara_handler.cpp`, `yara_handler.h`
- **Methods:**
    - `initialize()`: Initializes the YARA library and prints error information if initialization fails.
    - `setRulePath()`: Sets the YARA rule path directory.
    - `compileRuleFile()`: Compiles YARA rule files.
    - `scanFile(const std::string& filePath)`: Scans files based on compiled YARA rules.
    - `findYaraFiles(const std::string& directory)`: Returns the absolute paths of `.yar` extension detection rule files as a container.
    - `getMatchedRules()`: Retrieves matched rule information from the scan results.
    - `yaraCallbackFunction()`: Callback function to identify scan results.
    - `yaraCallbackFunctionWrapper()`: Static wrapper function for callback function processing in the `scanFile` method.

## 2. Main Logic Information

---

### 1) main Function

- **Primary Functionality:**
    - Controls the main flow of the program through user interface interactions.
    - Scans files based on user input, prints results, logs them, and deletes or quarantines files.
- **Source Code:** `main.cpp`
