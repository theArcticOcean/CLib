/**********************************************************
*
* @brief    This source file provides a way to print log into
*           local file in QT development environment.
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

static QString logFilePath;

void LogInit();

void print( const char * fmt, ... );

#endif // LOGPRINTER_H
