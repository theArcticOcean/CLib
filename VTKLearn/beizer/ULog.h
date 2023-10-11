#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <string>

#define DEBUG_OFF 0

#if DEBUG_OFF
    #define Log( Level, ... )
#else
    #define Log( Level, ... ) print( Level, __FILE__, __LINE__, __VA_ARGS__ )
#endif

//#define Log( Level, format, ... ) print( Level, __FUNCTION__, __LINE__, format, __VA_ARGS__ )

enum E_Log_Level
{
    IError,
    IWarning,
    IDebug,
    IInfo
};

template <class T>
void print(const T &t) {
    std::cout << t << std::endl;
}

template <class T, class... Args>
void print(T parameter, Args... rest) {
    std::cout <<  parameter;
    print(rest...);
}

template <class... Args>
void print(E_Log_Level level, const char *name, int line,  Args... args)
{
    std::string str;
    switch ( level )
    {
    case IError:
        str = "[Error]";
        break;
    case IWarning:
        str = "[Warning]";
        break;
    case IDebug:
        str = "[Debug]";
        break;
    case IInfo:
        str = "[Info]";
        break;
    }
    str = str + "[" + name + ":";
    str = str + std::to_string( line ) + "]\n";

    print( str, args...);
}

#endif // LOG_HPP
