#include <iostream>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <QDebug>
using namespace std;

int main()
{
    QString filePath = "/Users/weiyang/Desktop/test.json";
    QFile file( filePath );
    QJsonDocument document;
    QByteArray bytes;
    QJsonObject jsonObj;
    QJsonObject picturesObj;
    QJsonObject newPicturesObj;
    if( file.open( QIODevice::ReadOnly ) )
    {
        bytes = file.readAll();
        file.close();

        QJsonParseError jsonError;
        document = QJsonDocument::fromJson( bytes, &jsonError );
        if( jsonError.error != QJsonParseError::NoError )
        {
            return -1;
        }
        if( document.isObject() )
        {
            jsonObj = document.object();
            QString pictureKey = "picture";
            if( jsonObj.contains( pictureKey ) )
            {
                picturesObj = jsonObj.value( pictureKey ).toObject();
                QStringList pictureKeys = picturesObj.keys();
                for( auto key: pictureKeys )
                {
                    QString picPath = picturesObj.take( key ).toString();
                    picPath.replace( "Setting Data1/photo", "Photo" );
                    newPicturesObj.insert( key, picPath );
                }

                jsonObj.remove( pictureKey );
                jsonObj.insert( pictureKey, newPicturesObj );
            }
        }
    }

    if( newPicturesObj.size() > 0 )
    {
        document.setObject( jsonObj );
        QByteArray bytes = document.toJson( QJsonDocument::Indented );
        if( file.open( QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate ) )
        {
            QTextStream iStream( &file );
            iStream.setCodec( "utf-8" );
            iStream << bytes;
            file.close();
        }
        qDebug() << "Update finished.";
    }
    return 0;
}
