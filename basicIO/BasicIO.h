#ifndef BASICIO_H
#define BASICIO_H

#include <string>

class BasicIO
{
public:
    static bool CopyAFile(const std::string inFile, const std::string outFile);
    static bool RemoveFile(const std::string filePath);
    static void RemoveDir(const std::string folderPath);
    static void MoveAFile(const std::string oldFile, const std::string newFile);
    static void CheckFolderExist(const std::string folderPath);
    static bool FileExists( const std::string path );
    static void HandleLastSlash( std::string &path );
};

#endif // BASICIO_H
