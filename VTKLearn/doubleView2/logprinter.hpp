#ifndef LOGPRINTER_HPP
#define LOGPRINTER_HPP

#include <unistd.h>
#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QCoreApplication>

// QtGlobal has many basic function such as qCritical()
#include <QtGlobal>

#define BUFFER_SIZE 1024*10

static QString logFilePath;

/*
*   Create log file, truncate the file if it exsits.
*/
void LogInit()
{
    logFilePath = QCoreApplication::applicationDirPath();
    logFilePath = logFilePath + "/log.txt";
    if(  0 == access( logFilePath.toStdString().c_str(), F_OK )
         && truncate( logFilePath.toStdString().c_str(), 0 ) )
    {
        QString prefix = "[logInit] ";
        prefix = prefix + logFilePath;
        perror( prefix.toStdString().c_str() );
    }
}

/*
*   The function is provided for qInstallMessageHandler.
*   It redirects output messages to log file.
*/
void LogBase
    (
    QtMsgType type,
    const QMessageLogContext &context = QMessageLogContext( __FILE__, __LINE__, __FUNCTION__, NULL ),
    const QString &msg = QString("")
    )
{
    FILE *fp;
    int offset;
    char buffer[ BUFFER_SIZE ] = {0};
    QByteArray localMsg = msg.toLocal8Bit();

    fp = NULL;
    switch( type )
    {
    case QtDebugMsg:
    offset = sprintf( buffer, "[Debug ");
    break;
    case QtInfoMsg:
    offset = sprintf( buffer, "[Info ");
    break;
    case QtWarningMsg:
    offset = sprintf( buffer, "[Warning " );
    break;
    case QtCriticalMsg:
    offset = sprintf( buffer, "[Critical " );
    break;
    case QtFatalMsg:
    offset = sprintf( buffer, "[Fatal " );
    break;
    }

    sprintf( buffer + offset, "(%s, %u, %s)]\n%s\n\n", context.file, context.line, context.function, localMsg.constData() );
    fp = fopen( logFilePath.toStdString().c_str(), "a" );
    if( NULL != fp )
    {
        fprintf( fp, "%s", buffer );
        fclose( fp );
    }
}

#endif // LOGPRINTER_HPP
