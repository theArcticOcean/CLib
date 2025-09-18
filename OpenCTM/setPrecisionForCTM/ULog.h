// Disable warning messages 4507 and 4034.
//#pragma warning( disable : 4507 34 )

// Issue warning C4385 only once.
//#pragma warning( once : 4385 )

#ifndef ULOG_H
#define ULOG_H

#include <iostream>
#include <string>
#include <sstream>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <malloc.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#endif // __EMSCRIPTEN__

#ifndef WIN32

// -------------------------------------- for wasm ------------------------------------------
#ifdef LOG_OFF
#define LOG(format, ...)
#else
#define LOG( Level, ... ) print( Level, __FILE__, __LINE__, __VA_ARGS__ )
#endif

#define LOG_CLOUD( Level, ... ) writeAWSLog( Level, __FILE__, __LINE__, __VA_ARGS__ )

enum E_Log_Level
{
    ERR,
    WARNING,
    NOTICE,
    INFO,
    DEBUG
};

template <class T>
void print(const T& t) {
    std::cout << t << std::endl;
}

template <class T, class... Args>
void print(T parameter, Args... rest) {
    std::cout << parameter;
    print(rest...);
}

template <class... Args>
void print(E_Log_Level level, const char* name, int line, Args... args)
{
    std::string str;
    switch (level)
    {
    case ERR:
        str = "[Error]";
        break;
    case WARNING:
        str = "[Warning]";
        break;
    case DEBUG:
        str = "[Debug]";
        break;
    case INFO:
        str = "[Info]";
        break;
    case NOTICE:
        str = "[Notice]";
        break;
    }
    str = str + "[" + name + ":";
    str = str + std::to_string(line) + "] ";

    print(str, args...);
}

inline void formatArgsToString(std::stringstream&) {}

template <typename T, typename... Args>
void formatArgsToString(std::stringstream& ss, T&& first, Args&&... args) {
    ss << std::forward<T>(first);
    if constexpr (sizeof...(args) > 0) {
        ss << " ";
    }
    formatArgsToString(ss, std::forward<Args>(args)...);
}

template <typename... Args>
std::string argsToString(Args&&... args) {
    std::stringstream ss;
    formatArgsToString(ss, std::forward<Args>(args)...);
    return ss.str();
}

template <class... Args>
void writeAWSLog(E_Log_Level level, const char* name, int line, Args... args)
{
    std::string levelStr;
    switch (level)
    {
    case ERR:
        levelStr = "Error";
        break;
    case WARNING:
        levelStr = "Warning";
        break;
    case NOTICE:
        levelStr = "Notice";
        break;
    case DEBUG:
        levelStr = "Debug";
        break;
    case INFO:
        levelStr = "Info";
        break;
    default:
        break;
    }
    std::string logStr = "[" + levelStr + "] [" + name + ":" + std::to_string(line) + "] " + argsToString(args...);

    //call the js to send the log
#ifdef __EMSCRIPTEN__
    std::string jsCode = "writeAWSLogCallback";
    jsCode += "(";
    jsCode += ("'" + levelStr + "'");
    jsCode += " , ";
    jsCode += ("'" + logStr + "'");
    jsCode += ");";
    std::cout << "writeAWSLog jsCode is " << jsCode << std::endl;
    //emscripten_run_script(jsCode.c_str());
    EM_ASM({
        if (typeof writeAWSLogCallback == = 'function') {
            writeAWSLogCallback(UTF8ToString($0), UTF8ToString($1)); // 自动处理转义
        }
 else {
  console.warn("writeAWSLogCallback is not defined!");
}
        }, levelStr.data(), logStr.data());
#endif
}

#else

// ------------------------------------------ for windows -----------------------------------------------
#define ERR             0
#define WARNING         1
#define NOTICE          2
#define INFO            3
#define DEBUG           4

//#define LOG_OFF         1

#ifdef LOG_OFF
#define LOG(format, ...)
#else//!LOG_OFF
#define LOG(level, ...) print(level, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif

#define LOG_CLOUD( level, ... )

template<typename T>
void printA(const T& t)
{
    std::cout << t;
}
template<typename... Args>
void print(const Args& ...rest)
{
#ifdef _WIN64
    auto arr = { 0, (printA(rest), 0)... };
    std::cout << std::endl;
#endif
}

template<typename... Args>
void print(const int priority, const char* func, const int line, const Args& ...rest)
{
    int maxLevel = INFO;
    char* envBuf = nullptr;
    size_t envSize = 0;
#ifdef _WIN64
    if (_dupenv_s(&envBuf, &envSize, "LOG_LEVEL") == 0 && envBuf != nullptr)
    {
        maxLevel = atoi(envBuf);
        free(envBuf);
    }
#endif
    if (priority > maxLevel) return;
    switch (priority)
    {
    case DEBUG:
        std::cout << "[DEBUG]";
        break;
    case INFO:
        std::cout << "[INFO]";
        break;
    case NOTICE:
        std::cout << "[NOTICE]";
        break;
    case WARNING:
        std::cout << "[WARNING]";
        break;
    case ERR:
        std::cout << "[ERR]";
        break;
    default:
        break;
    }

    std::cout << "[" << func << "-" << line << "]: ";
#ifdef _WIN64
    print(rest...);
#endif
}

#endif

#endif//ULOG_H
