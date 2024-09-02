#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <filesystem>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <chrono>

namespace fs = std::filesystem;

class Shell {

public:
    void run() {
        std::string input;
        while (true) {
            std::cout << "MyShell> ";
            std::getline(std::cin, input);

            if (input == "exit") {
                break;
            }

            executeCommand(input);
        }
    }

private:
    void executeCommand(const std::string& input) {
        std::vector<std::string> tokens = tokenize(input);

        if (tokens.empty()) {
            return;
        }

        std::string command = tokens[0];
        if (command == "cd") {
            changeDirectory(tokens);
        } else if (command == "ls") {
            listDirectory(tokens);
        } else if (command == "mv") {
            moveFile(tokens);
        } else if (command == "rm") {
            removeFile(tokens);
        } else if (command == "cp") {
            copyFile(tokens);
        } else {
            std::cout << "Unknown command: " << command << std::endl;
        }
    }

    std::vector<std::string> tokenize(const std::string& input) {
        std::vector<std::string> tokens;
        std::istringstream iss(input);
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

     void changeDirectory(const std::vector<std::string>& tokens) {
        if (tokens.size() < 2) {
            std::cout << "Usage: cd [options] <directory>" << std::endl;
            return;
        }

        std::string option = tokens[1];

        if (option == "--help") {
            displayCdHelp();
            return;
        }

        fs::path targetDir;

        if (option == "~" || option == "~username") {
            // Go to home directory or specified user's home directory
            targetDir = getHomeDirectory(option);
        } else if (option == ".") {
            // Stay in the current directory
            targetDir = ".";
        } else {
            // Go to a subdirectory
            targetDir = option;
        }

        if (fs::exists(targetDir) && fs::is_directory(targetDir)) {
            fs::current_path(targetDir);
        } else {
            std::cout << "Directory does not exist: " << targetDir.string() << std::endl;
        }
    }

    void displayCdHelp() {
        std::cout << "Usage: cd [options] <directory>" << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "  ~ or ~username      Go to home directory or specified user's home directory" << std::endl;
        std::cout << "  .                   Stay in the current directory" << std::endl;
        std::cout << "  dir                 Go to a subdirectory" << std::endl;
        std::cout << "  --help              Display this help message" << std::endl;
    }

    fs::path getHomeDirectory(const std::string& option) {
        // Get the home directory path
        if (option == "~") {
            return fs::path(getenv("HOME"));
        } else if (option.substr(0, 2) == "~/") {
            // Get the home directory of the specified user
            std::string username = option.substr(2);
            return fs::path("/home/" + username);
        } else {
            return fs::path(getenv("HOME")); // Default to user's home directory
        }
    }


        void listDirectory(const std::vector<std::string>& tokens) {
        fs::path dirPath = ".";
        bool longFormat = false;
        bool reverseOrder = false;
        bool recursive = false;

        // Check for options
        for (size_t i = 1; i < tokens.size(); ++i) {
            if (tokens[i] == "-l") {
                longFormat = true;
            } else if (tokens[i] == "-r") {
                reverseOrder = true;
            } else if (tokens[i] == "-R") {
                recursive = true;
            } else if (tokens[i] == "~") {
                dirPath = getenv("HOME");
            } else if (tokens[i] == "../") {
                dirPath = "..";
            } else if (tokens[i] == "--help") {
                displayLsHelp();
                return;
            } else {
                dirPath = tokens[i];
            }
        }

        try {
            if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
                if (recursive) {
                    listDirectoryRecursive(dirPath, longFormat, reverseOrder);
                } else {
                    listDirectorySimple(dirPath, longFormat, reverseOrder);
                }
            } else {
                std::cout << "Directory does not exist: " << dirPath.string() << std::endl;
            }
        } catch (const fs::filesystem_error& ex) {
            std::cout << "Error: " << ex.what() << std::endl;
        }
    }

    void listDirectorySimple(const fs::path& dirPath, bool longFormat, bool reverseOrder) {
        std::vector<fs::path> entries;

        for (const auto& entry : fs::directory_iterator(dirPath)) {
            entries.push_back(entry.path());
        }

        if (reverseOrder) {
            std::reverse(entries.begin(), entries.end());
        }

        for (const auto& entry : entries) {
            if (longFormat) {
                displayLongFormat(entry);
            } else {
                std::cout << entry.filename().string() << std::endl;
            }
        }
    }

    void listDirectoryRecursive(const fs::path& dirPath, bool longFormat, bool reverseOrder) {
        for (const auto& entry : fs::recursive_directory_iterator(dirPath)) {
            if (longFormat) {
                displayLongFormat(entry.path());
            } else {
                std::cout << entry.path().string() << std::endl;
            }
        }
    }

    void displayLongFormat(const fs::path& filePath) {
        try {
            auto status = fs::status(filePath);
            std::string type;
            std::string perms;

            switch (status.type()) {
                case fs::file_type::regular:
                    type = "-";
                    break;
                case fs::file_type::directory:
                    type = "d";
                    break;
                case fs::file_type::symlink:
                    type = "l";
                    break;
                default:
                    type = "?";
                    break;
            }

            perms += (status.permissions() & fs::perms::owner_read) != fs::perms::none ? "r" : "-";
            perms += (status.permissions() & fs::perms::owner_write) != fs::perms::none ? "w" : "-";
            perms += (status.permissions() & fs::perms::owner_exec) != fs::perms::none ? "x" : "-";
            perms += (status.permissions() & fs::perms::group_read) != fs::perms::none ? "r" : "-";
            perms += (status.permissions() & fs::perms::group_write) != fs::perms::none ? "w" : "-";
            perms += (status.permissions() & fs::perms::group_exec) != fs::perms::none ? "x" : "-";
            perms += (status.permissions() & fs::perms::others_read) != fs::perms::none ? "r" : "-";
            perms += (status.permissions() & fs::perms::others_write) != fs::perms::none ? "w" : "-";
            perms += (status.permissions() & fs::perms::others_exec) != fs::perms::none ? "x" : "-";

            auto lastWriteTime = fs::last_write_time(filePath);
            auto writeTime = std::chrono::time_point_cast<std::chrono::system_clock::duration>(lastWriteTime);
           // auto time_t_writeTime = std::chrono::system_clock::to_time_t(writeTime);

            std::cout << type << perms << " "
                      //<< fs::hard_link_count(status) << " "
                      << fs::file_size(filePath) << " "
                     // << std::put_time(std::localtime(&time_t_writeTime), "%m/%d %H:%M") << " "
                      << filePath.filename().string() << std::endl;
        } catch (const fs::filesystem_error& ex) {
            std::cout << "Error: " << ex.what() << std::endl;
        }
    }

    void displayLsHelp() {
        std::cout << "Usage: ls [options] <directory>" << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "  -l                Show list in long format" << std::endl;
        std::cout << "  -r                Print list in reverse order" << std::endl;
        std::cout << "  -R                Display content of sub-directories also" << std::endl;
        std::cout << "  ~                 Give the contents of home directory" << std::endl;
        std::cout << "  ../               Give the contents of parent directory" << std::endl;
        std::cout << "  --help            Display this help message" << std::endl;
    }

    void moveFile(const std::vector<std::string>& tokens) {
        if (tokens.size() < 3) {
            std::cout << "Usage: mv [options] <source> <destination>" << std::endl;
            return;
        }

        std::vector<std::string> options;
        fs::path source;
        fs::path destination;

        // Separate options, source, and destination
        size_t i = 1;
        while (i < tokens.size() && tokens[i].substr(0, 1) == "-") {
            options.push_back(tokens[i]);
            ++i;
        }

        if (i < tokens.size()) {
            source = tokens[i];
            ++i;
        } else {
            std::cout << "Usage: mv [options] <source> <destination>" << std::endl;
            return;
        }

        if (i < tokens.size()) {
            destination = tokens[i];
        } else {
            std::cout << "Usage: mv [options] <source> <destination>" << std::endl;
            return;
        }

        performMove(options, source, destination);
    }

    void performMove(const std::vector<std::string>& options, const fs::path& source, const fs::path& destination) {
        try {
            bool interactive = false;
            bool wildcard = false;
            bool suffixBackup = false;
            bool onlyIfNotExists = false;

            // Process options
            for (const std::string& opt : options) {
                if (opt == "-i") {
                    interactive = true;
                } else if (opt == "*") {
                    wildcard = true;
                } else if (opt == "--suffix") {
                    suffixBackup = true;
                } else if (opt == "-u") {
                    onlyIfNotExists = true;
                } else if (opt == "--help") {
                    std::cout << "Usage: mv [options] <source> <destination>" << std::endl;
                    std::cout << "Options:" << std::endl;
                    std::cout << "  -i            Ask for permission to overwrite" << std::endl;
                    std::cout << "  *             Move multiple files to a specific directory" << std::endl;
                    std::cout << "  --suffix      Take backup before overwriting" << std::endl;
                    std::cout << "  -u            Only move those files that don't exist" << std::endl;
                    return;
                } else {
                    std::cout << "Unknown option: " << opt << std::endl;
                    return;
                }
            }

            if (wildcard) {
                moveFilesWithWildcard(source, destination);
            } else {
                moveSingleFile(source, destination, interactive, suffixBackup, onlyIfNotExists);
            }
        } catch (const fs::filesystem_error& ex) {
            std::cout << "Error: " << ex.what() << std::endl;
        }
    }

    void moveSingleFile(const fs::path& source, const fs::path& destination, bool interactive, bool suffixBackup, bool onlyIfNotExists) {
        if (onlyIfNotExists && fs::exists(destination / source.filename())) {
            std::cout << "File already exists at the destination: " << destination.string() << std::endl;
            return;
        }

        if (interactive && fs::exists(destination / source.filename())) {
            std::string response;
            std::cout << "Destination file already exists. Overwrite? (y/n): ";
            std::getline(std::cin, response);
            if (response != "y") {
                std::cout << "Move operation canceled." << std::endl;
                return;
            }
        }

        fs::path destinationPath = destination / source.filename();

        if (suffixBackup) {
            // Create a backup by appending a suffix to the filename
            fs::path backupPath = destination / (source.filename().stem().string() + "_backup" + source.filename().extension().string());
            fs::rename(destinationPath, backupPath);
        }

        fs::rename(source, destinationPath);
        std::cout << "Moved: " << source << " to " << destination << std::endl;
    }

    void moveFilesWithWildcard(const fs::path& source, const fs::path& destination) {
        for (const auto& entry : fs::directory_iterator(source)) {
            moveSingleFile(entry.path(), destination, false, false, false);
        }
    }

    void removeFile(const std::vector<std::string>& tokens) {
        if (tokens.size() < 2) {
            std::cout << "Usage: rm [options] <file/directory>" << std::endl;
            return;
        }

        std::vector<std::string> options;
        fs::path fileOrDir;

        // Separate options and file/directory
        size_t i = 1;
        while (i < tokens.size() && tokens[i].substr(0, 1) == "-") {
            options.push_back(tokens[i]);
            ++i;
        }

        if (i < tokens.size()) {
            fileOrDir = tokens[i];
        } else {
            std::cout << "Usage: rm [options] <file/directory>" << std::endl;
            return;
        }

        performRemove(options, fileOrDir);
    }

    void performRemove(const std::vector<std::string>& options, const fs::path& fileOrDir) {
        try {
            bool recursive = false;
            bool interactive = false;
            bool force = false;
            bool forceRecursive = false;

            // Process options
            for (const std::string& opt : options) {
                if (opt == "-r" || opt == "-R") {
                    recursive = true;
                } else if (opt == "-i") {
                    interactive = true;
                } else if (opt == "-rf") {
                    forceRecursive = true;
                } else if (opt == "-f") {
                    force = true;
                } else if (opt == "--help") {
                    std::cout << "Usage: rm [options] <file/directory>" << std::endl;
                    std::cout << "Options:" << std::endl;
                    std::cout << "  -r, -R        Remove directory recursively" << std::endl;
                    std::cout << "  -i            Remove file interactively" << std::endl;
                    std::cout << "  -rf           Remove directory forcefully" << std::endl;
                    std::cout << "  -f            Force removal, ignores non-existent files and overrides prompts" << std::endl;
                    return;
                } else {
                    std::cout << "Unknown option: " << opt << std::endl;
                    return;
                }
            }

            if (recursive && forceRecursive) {
                std::cout << "Error: Options -r and -rf are mutually exclusive." << std::endl;
                return;
            }

            if (forceRecursive) {
                removeDirectoryForcefully(fileOrDir);
            } else if (recursive) {
                removeDirectoryRecursively(fileOrDir);
            } else if (force) {
                removeFileForcefully(fileOrDir);
            } else if (interactive) {
                removeFileInteractively(fileOrDir);
            } else {
                removeFileOrDirectory(fileOrDir);
            }
        } catch (const fs::filesystem_error& ex) {
            std::cout << "Error: " << ex.what() << std::endl;
        }
    }

    void removeFileOrDirectory(const fs::path& fileOrDir) {
        if (fs::exists(fileOrDir)) {
            if (fs::is_directory(fileOrDir)) {
                fs::remove(fileOrDir);
                std::cout << "Removed directory: " << fileOrDir << std::endl;
            } else {
                fs::remove(fileOrDir);
                std::cout << "Removed file: " << fileOrDir << std::endl;
            }
        } else {
            std::cout << "File or directory does not exist: " << fileOrDir << std::endl;
        }
    }

    void removeFileInteractively(const fs::path& file) {
        if (fs::exists(file)) {
            std::string response;
            std::cout << "Are you sure you want to remove '" << file << "'? (y/n): ";
            std::getline(std::cin, response);
            if (response == "y") {
                fs::remove(file);
                std::cout << "Removed file: " << file << std::endl;
            } else {
                std::cout << "Removal canceled." << std::endl;
            }
        } else {
            std::cout << "File does not exist: " << file << std::endl;
        }
    }

    void removeFileForcefully(const fs::path& file) {
        if (fs::exists(file)) {
            fs::remove(file);
            std::cout << "Removed file: " << file << std::endl;
        } else {
            std::cout << "File does not exist: " << file << std::endl;
        }
    }

    void removeDirectoryRecursively(const fs::path& dir) {
        if (fs::exists(dir) && fs::is_directory(dir)) {
            fs::remove_all(dir);
            std::cout << "Removed directory recursively: " << dir << std::endl;
        } else {
            std::cout << "Directory does not exist: " << dir << std::endl;
        }
    }

    void removeDirectoryForcefully(const fs::path& dir) {
        try {
            if (fs::exists(dir) && fs::is_directory(dir)) {
                fs::remove_all(dir);
                std::cout << "Removed directory forcefully: " << dir << std::endl;
            } else {
                std::cout << "Directory does not exist: " << dir << std::endl;
            }
        } catch (const fs::filesystem_error& ex) {
            std::cout << "Error: " << ex.what() << std::endl;
        }
    }


    void copyFile(const std::vector<std::string>& tokens) {
        if (tokens.size() < 3) {
            std::cout << "Usage: cp [options] <source> <destination>" << std::endl;
            return;
        }

        std::vector<std::string> options;
        fs::path source;
        fs::path destination;

        // Separate options, source, and destination
        size_t i = 1;
        while (i < tokens.size() && tokens[i].substr(0, 1) == "-") {
            options.push_back(tokens[i]);
            ++i;
        }

        if (i < tokens.size()) {
            source = tokens[i];
            ++i;
        } else {
            std::cout << "Usage: cp [options] <source> <destination>" << std::endl;
            return;
        }

        if (i < tokens.size()) {
            destination = tokens[i];
        } else {
            std::cout << "Usage: cp [options] <source> <destination>" << std::endl;
            return;
        }

        performCopy(options, source, destination);
    }

    void performCopy(const std::vector<std::string>& options, const fs::path& source, const fs::path& destination) {
        try {
            bool copyContents = false;
            bool dereference = false;
            bool linkFiles = false;
            bool recursive = false;

            // Process options
            for (const std::string& opt : options) {
                if (opt == "--copy-contents") {
                    copyContents = true;
                } else if (opt == "-d") {
                    dereference = true;
                } else if (opt == "--link" || opt == "-l") {
                    linkFiles = true;
                } else if (opt == "--recursive" || opt == "-r" || opt == "-R") {
                    recursive = true;
                } else if (opt == "--help") {
                    std::cout << "Usage: cp [options] <source> <destination>" << std::endl;
                    std::cout << "Options:" << std::endl;
                    std::cout << "  --copy-contents       Copy special file contents when recursive" << std::endl;
                    std::cout << "  -d                    Equivalent to --no-dereference --preserve=links" << std::endl;
                    std::cout << "  --link, -l            Specify hard link files rather than copying" << std::endl;
                    std::cout << "  --recursive, -r, -R   Recursively copy directories" << std::endl;
                    return;
                } else {
                    std::cout << "Unknown option: " << opt << std::endl;
                    return;
                }
            }

            if (linkFiles && recursive) {
                std::cout << "Error: Options --link and --recursive are mutually exclusive." << std::endl;
                return;
            }

            if (dereference) {
                fs::copy_options copyOptions = fs::copy_options::none;
                if (!copyContents) {
                    copyOptions |= fs::copy_options::skip_symlinks;
                }
                if (linkFiles) {
                    copyOptions |= fs::copy_options::create_hard_links;
                }

                fs::copy(source, destination, copyOptions);
            } else {
                if (recursive) {
                    fs::copy(source, destination, fs::copy_options::recursive);
                } else {
                    fs::copy(source, destination);
                }
            }

            std::cout << "Copied: " << source << " to " << destination << std::endl;
        } catch (const fs::filesystem_error& ex) {
            std::cout << "Error: " << ex.what() << std::endl;
        }
    }
};

int main() {
    Shell myShell;
    myShell.run();
    return 0;
}
