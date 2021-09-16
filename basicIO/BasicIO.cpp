#include "BasicIO.h"
#include "log.hpp"

#ifdef WIN32
#include "dirent_win.h"
#else
#include <locale.h>
#include <fstream>
#include <unistd.h>
#endif

#include <fstream>
#include <direct.h>
#include <stdio.h>
#include <filesystem>

bool BasicIO::CopyAFile(const std::string inFile, const std::string outFile)
{
    if ( !FileExists( inFile.c_str() ) )
    {
        Log( IError, "file doesn't exist ", inFile );
        return false;
    }
    std::ifstream  src(inFile, std::ios::binary);
    std::ofstream  dst(outFile, std::ios::binary);
    dst << src.rdbuf();
    return true;
}

bool BasicIO::RemoveFile(const std::string filePath)
{
    if ( !FileExists( filePath.c_str() ) )
    {
        return true;
    }
    if( 0 == remove( filePath.c_str() ) )
    {
        return true;
    }
    return false;
}

void BasicIO::RemoveDir(const std::string folderPath)
{
    struct dirent *entry;
    DIR *dir = opendir(folderPath.c_str());
    if (nullptr == dir)
    {
        return ;
    }
    while (nullptr != (entry = readdir(dir)))
    {
        std::string _folderPath = folderPath;
        HandleLastSlash(_folderPath);

        std::string childPath = _folderPath + "/" + entry->d_name;
        HandleLastSlash( childPath );

        if( strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 ||
            strcmp(entry->d_name, "./") == 0 || strcmp(entry->d_name, "../") == 0 )
        {
            continue;
        }
        if( DT_DIR == entry->d_type )
        {
            RemoveDir( childPath.c_str() );
        }
        else
        {
            RemoveFile( childPath.c_str() );
        }
    }
    rmdir( folderPath.c_str() );
    closedir(dir);
}

void BasicIO::MoveAFile(const std::string oldFile, const std::string newFile)
{
    if ( FileExists( oldFile ) )
    {
        CopyAFile( oldFile.c_str(), newFile.c_str() );
        if (oldFile != newFile)
        {
            RemoveFile( oldFile.c_str() );
        }
    }
}

void BasicIO::CheckFolderExist(const std::string folderPath)
{
    if( !FileExists(folderPath) )
    {
        Log( IInfo, "create folder: ", folderPath );
        mkdir( folderPath.c_str() );
    }
}

/*
*   Checks if the given file status or path corresponds to an existing file or directory.
*/
bool BasicIO::FileExists(const std::string path)
{
    return std::filesystem::exists( path.c_str() );

    // C++
    //std::ifstream file( path.c_str() );
    //return file.good();

    /* fast way
    FILE * fp = fopen(path.c_str(), "r");
    if (fp != nullptr)
    {
        fclose(fp);
        return true;
    }
    return false; */
}

void BasicIO::HandleLastSlash(std::string &path)
{
    while ( path[path.size()-1] == '/' ) {
        path = path.substr( 0, path.size()-1 );
    }
    while ( path[path.size()-1] == '\\' ) {
        path = path.substr( 0, path.size()-1 );
    }
}
