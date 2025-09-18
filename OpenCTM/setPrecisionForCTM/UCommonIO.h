#pragma once

#include "UMacroDefinition.h"

#include <string>
#include <iostream>
#include <string.h>

class UDEF_Class UCommonIO
{
public:
    UCommonIO();

    bool CopyAFile(const std::string inFile, const std::string outFile);
    bool RemoveFile(const std::string filePath);
    void RemoveDir(const std::string folderPath);
    void MoveAFile(const std::string oldFile, const std::string newFile);
    bool CreateFolder(const std::string& path);
    bool CreateDirectories(const std::string& path);
    bool CreateDirForFilePath(const std::string& path);

    static void CheckFolderExist(const std::string folderPath);
    static bool FileExists(const std::string path);
    static void HandleLastSlash(std::string& path);
    static void StringReplaceAll(std::string& str, const std::string& old_substr, const std::string& new_substr);
};