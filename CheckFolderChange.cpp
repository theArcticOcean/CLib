#include <iostream>
#include <string>
#include <numeric>
#include <set>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <QJsonDocument>
#include <QByteArray>
#include <QFile>
#include <QTextStream>
#include <QJsonObject>
#include <QDebug>
#include <QJsonParseError>
#include <QDir>
#include <QDebug>
#include <QCryptographicHash>
using namespace std;

QList<QByteArray> sha256List1;
QList<QByteArray> sha256List2;

void CheckShaSum( const QDir &dir )
{
    QFileInfoList entries = dir.entryInfoList(QDir::AllEntries|QDir::NoDotAndDotDot, QDir::Name);
    for( auto fileInfo: entries )
    {
        if( fileInfo.isDir() )
        {
            CheckShaSum( QDir( fileInfo.absoluteFilePath() ) );
        }
        else
        {
            QFile file( fileInfo.filePath() );
            if( file.open( QIODevice::ReadOnly ) )
            {
                QByteArray bytes = file.readAll();
                // the check sum is `sha256 result` + `file path string`
                QByteArray hashValue = QCryptographicHash::hash( bytes, QCryptographicHash::Sha256 );
                hashValue += fileInfo.filePath();
                sha256List1.push_back( hashValue );
                file.close();
            }
        }
    }
}

int main(int argc, char** argv)
{
    QString path = "/Users/weiyang/Desktop/files";
    CheckShaSum( QDir( path ) );
    for( auto it: sha256List1 )
    {
        qDebug() << it;
    }
    for( int i = 0; i < sha256List1.size(); ++i )
    {
        for( int j = i+1; j < sha256List1.size(); ++j )
        {
            if( sha256List1[i] == sha256List1[j] )
            {
                qDebug() << "there is same file";
            }
        }
    }

/* // change txt1
    QFile file( "/Users/weiyang/Desktop/files/txt1" );
    if( file.open( QIODevice::ReadWrite ) )
    {
        QTextStream istream( &file );
        istream << "world";
        file.close();
    }

    for( int i = 0; i < entries.size(); ++i )
    {
        QFileInfo fileInfo = entries[i];
        if( "." != fileInfo.fileName() && ".." != fileInfo.fileName() )
        {
            qDebug() << fileInfo.filePath();
            QFile file( fileInfo.filePath() );
            if( file.open( QIODevice::ReadOnly ) )
            {
                QByteArray bytes = file.readAll();
                QByteArray hashValue = QCryptographicHash::hash( bytes, QCryptographicHash::Sha256 );
                qDebug() << hashValue;
                sha256List2.push_back( hashValue );
                file.close();
            }
        }
    }
    for( int i = 0; i < sha256List1.size(); ++i )
    {
        qDebug() << ( sha256List1[i] == sha256List2[i] );
    } */
    return 0;
}
