

#ifdef WIN32
#include <direct.h>
#include "dirent_win.h"
#include <io.h>
#else
#include <dirent.h>
#include <locale.h>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#endif
#include "UCommonIO.h"
#include "ULog.h"
#include <stdio.h>
#include <filesystem>

UCommonIO::UCommonIO()
{

}

bool UCommonIO::CopyAFile(const std::string inFile, const std::string outFile)
{
    if (!FileExists(inFile.c_str()))
    {
        LOG(ERR, "file doesn't exist ", inFile);
        return false;
    }
    std::ifstream  src(inFile, std::ios::binary);
    std::ofstream  dst(outFile, std::ios::binary);
    dst << src.rdbuf();
    return true;
}

bool UCommonIO::RemoveFile(const std::string filePath)
{
    if (!FileExists(filePath.c_str()))
    {
        return true;
    }
    if (0 == remove(filePath.c_str()))
    {
        return true;
    }
    return false;
}

void UCommonIO::RemoveDir(const std::string folderPath)
{
    struct dirent* entry;
    DIR* dir = opendir(folderPath.c_str());
    if (nullptr == dir)
    {
        return;
    }
    while (nullptr != (entry = readdir(dir)))
    {
        std::string _folderPath = folderPath;
        HandleLastSlash(_folderPath);

        std::string childPath = _folderPath + "/" + entry->d_name;
        HandleLastSlash(childPath);

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 ||
            strcmp(entry->d_name, "./") == 0 || strcmp(entry->d_name, "../") == 0)
        {
            continue;
        }
        if (DT_DIR == entry->d_type)
        {
            RemoveDir(childPath.c_str());
        }
        else
        {
            RemoveFile(childPath.c_str());
        }
    }
    rmdir(folderPath.c_str());
    closedir(dir);
}

void UCommonIO::MoveAFile(const std::string oldFile, const std::string newFile)
{
    if (FileExists(oldFile))
    {
        CopyAFile(oldFile.c_str(), newFile.c_str());
        if (oldFile != newFile)
        {
            RemoveFile(oldFile.c_str());
        }
    }
}

bool UCommonIO::CreateDirForFilePath(const std::string& path)
{
    size_t pos = 0, found;
    found = path.find_last_of('/', std::string::npos);
    if (found != std::string::npos)
    {
        std::string folder = path.substr(0, found);
        if (folder.empty() == false)
        {
            LOG(INFO, "found folder: ", folder);
            return CreateDirectories(folder);
        }
    }
    return true;
}

bool UCommonIO::CreateDirectories(const std::string& path) {
    size_t current = 0, found;
    while ((found = path.find_first_of('/', current)) != std::string::npos) {
        std::string sub_path = path.substr(0, found++);
        current = found;
        if (sub_path.empty()) continue;
        if (!CreateFolder(sub_path)) {
            return false;
        }
    }
    return CreateFolder(path);
}

bool UCommonIO::CreateFolder(const std::string& path) {
#ifdef WIN32
    int ret = mkdir(path.c_str());
    return (ret == 0 || errno == EEXIST);
#else
    int status = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (status == 0) {
        LOG(INFO, "created dir: ", path);
        return true;
    }
    else {
        if (errno == EEXIST) {
            return true;
        }
        else {
            LOG(ERR, "failed to create dir: ", path, ", ", strerror(errno));
            return false;
        }
    }
#endif
}

void UCommonIO::CheckFolderExist(const std::string folderPath)
{
    if (!FileExists(folderPath))
    {
        LOG(INFO, "create folder: ", folderPath);
#ifdef WIN32
        mkdir(folderPath.c_str());
#else
        mkdir(folderPath.c_str(), 0777);
#endif
    }
}

/*
*   Checks if the given file status or path corresponds to an existing file or directory.
*/
bool UCommonIO::FileExists(const std::string path)
{
    //return std::filesystem::exists( path.c_str() );

    //C++11
    //std::ifstream file( path.c_str() );
    //return file.good();

#ifdef WIN32
    return (_access(path.c_str(), 0) == 0);
#else
    std::ifstream iStream;
    iStream.open(path, ios::binary);
    if (!iStream.is_open())
    {
        LOG(ERR, "Open File error: ", path);
        return false;
    }
    iStream.close();
    return true;
#endif
}

void UCommonIO::HandleLastSlash(std::string& path)
{
    if (path == "") return;

    while (path[path.size() - 1] == '/') {
        path = path.substr(0, path.size() - 1);
    }
    while (path[path.size() - 1] == '\\') {
        path = path.substr(0, path.size() - 1);
    }
}

void UCommonIO::StringReplaceAll(std::string& str, const std::string& old_substr, const std::string& new_substr)
{
    size_t pos = 0;
    while ((pos = str.find(old_substr, pos)) != std::string::npos) {
        str.replace(pos, old_substr.length(), new_substr);
        pos += new_substr.length();
    }
}
