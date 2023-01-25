#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <string>
#include <vector>

class FileManager {
    using fileContent = std::vector<int>;
    private:
        static const std::string Path;
    public:
        static fileContent read (const std::string &fileName);
        static void write (const std::string &fileName, const fileContent &content);
};

#endif // FILE_MANAGER_HPP