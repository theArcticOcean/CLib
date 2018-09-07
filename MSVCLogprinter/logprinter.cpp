#include "logprinter.h"
#include <stdio.h>

void LogInit()
{
    logFilePath = QCoreApplication::applicationDirPath();
    logFilePath = logFilePath + "/log.txt";
    if(  0 == _access( logFilePath.toStdString().c_str(), 0 ) )
    {
        _unlink( logFilePath.toStdString().c_str() );
    }
}

void print(const char *file, const int line, const char *fmt, ...)
{
    const int BUFFSIZE = 1024;
    char buffer[ BUFFSIZE ] = {0};
    int offset = 0;
    FILE *fp = NULL;

    offset = snprintf( buffer + offset, BUFFSIZE - offset, "[%s, %d]\n", file, line );
    va_list vap;
    va_start( vap, fmt );  //variable arguments list vap init
    vsnprintf( buffer + offset, BUFFSIZE - offset, fmt, vap ); // add list vap to buff
    va_end( vap );

    fp = fopen( logFilePath.toStdString().c_str(), "a" );
    if( NULL != fp )
    {
        fprintf( fp, "%s", buffer );
        fclose( fp );
    }
}
