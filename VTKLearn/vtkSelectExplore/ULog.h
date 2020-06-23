#pragma once
#include <iostream>
#include <chrono>
#include <QtGlobal>
using namespace std::chrono;

#define ERR             0
#define WARNING         1
#define NOTICE          2
#define INFO            3
#define DEBUG           4

#ifdef LOG_OFF
#define LOG(format, ...)
#else // !LOG_OFF
#define LOG(level, format, ...) print(level, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

template<typename T, typename...Args>
void print(int priority, const char* func, int line, const T &t, const Args&...rest)
{
  int level;
  char *_buf = nullptr;
  size_t sz = 0;
#ifdef Q_OS_WIN
  if (_dupenv_s(&_buf, &sz, "LOG_LEVEL") == 0 && _buf != nullptr)
  {
    level = atoi(_buf);
    free(_buf);
  }
  else
    level = INFO;

  if (level < priority)
    return;
#endif
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
    #ifdef Q_OS_WIN
  print(t, rest...);
    #endif
}

template<typename T, typename...Args>
void print(const T &t, const Args&...rest)
{
  std::cout << t;
#ifdef Q_OS_WIN
  print(rest...);
#endif

}

template<typename T>
void print(const T &t)
{
  std::cout << t << std::endl;
}
#endif // LOG_OFF

//
typedef system_clock::time_point TS;
#define TIME_NOW() system_clock::now()
template<typename T>
double TimeSpend(T t1, T t2)
{
    auto duration = duration_cast<microseconds>(t2-t1);
    double spendTime = (double)(duration.count())*microseconds::period::num / microseconds::period::den;
    return spendTime;
}
