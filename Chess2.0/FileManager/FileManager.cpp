#include "FileManager.hpp"
#include <fstream>

const std::string FileManager::Path = "Chess2.0/FileManager/";

FileManager::fileContent FileManager::read (const std::string &fileName) {
    fileContent text;
    std::fstream file;
    file.open(Path + fileName, std::ios::in);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {   
            text.push_back(stoi(line)); //s to i
        }
        file.close();
    }
    return text;
}

void FileManager::write (const std::string &fileName, const fileContent &content) {
    fileContent text = read(fileName);

    std::fstream file;
    file.open(Path + fileName, std::ios::out);

    if (file.is_open()) {
        for (const auto &line : content) {
            file << line << std::endl;
        }
        file.close();
    }
}