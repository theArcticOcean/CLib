/**********************************************************
*
* @brief    This source file provides a way to print log into
*           local file in MSVC development environment.
*
* @author   theArcticOcean
***********************************************************/

#ifndef LOGPRINTER_H
#define LOGPRINTER_H

#include <stdarg.h>
#include <QDebug>
#include <stdio.h>
#include <QString>
#include <windows.h>
#include <io.h>
#include <QCoreApplication>

#if _MSC_VER
#define snprintf _snprintf
#endif

static QString logFilePath;

void LogInit();

void print( const char* file, const int line, const char * fmt, ...);

#define printer(...) print( __FILE__, __LINE__, __VA_ARGS__ )

#endif // LOGPRINTER_H
