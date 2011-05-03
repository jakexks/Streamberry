#include "albumartgetter.h"
#include "crossplatform.h"
#include <QtNetwork>
#include <QDebug>
#include <QRegExp>
#include <QFile>

AlbumArtGetter::AlbumArtGetter()
{
    LAST_API_KEY = "87b539ee580f14db8f9fe67c87a20760";
}

int AlbumArtGetter::get(QString Artist, QString Album)
{
    QString url = "http://ws.audioscrobbler.com/2.0/?method=album.getinfo";
    url += "&api_key=";
    url += LAST_API_KEY;
    url += "&artist=";
    url += Artist;
    url += "&album=";
    url += Album;
    filename = "/home/jakexks/";
    filename += Artist.toLower();
    filename += "-";
    filename += Album.toLower();
    filename += ".png";
    reply = qnam.get(QNetworkRequest(QUrl(url)));
    connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
    return 0;
}

void AlbumArtGetter::httpFinished()
{
    qDebug() << "FINISHED";
    //qDebug() << received;
    if (received.contains("<lfm status=\"failed\">"))
    {
        qDebug() << "Could not find album";
    }
    else
    {
        QStringList split = received.split('\n');
        QRegExp image("<image size=\"extralarge\">(http://.*png)</image>");

        for (int i = 0; i < split.length(); i++)
        {
            if (image.indexIn(split.at(i)) != -1)
            {
                downloadImage(image.cap(1));
                break;
            }
        }

    }
}

void AlbumArtGetter::httpReadyRead()
{
    qDebug() << "data ready to read";
    received.append(QString(reply->readAll()));
}

int AlbumArtGetter::downloadImage(QString imgurl)
{
    imagefile = new QFile(filename);
    if(!imagefile->open(QIODevice::WriteOnly))
    {
        qDebug()<<"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
    }
    imgreply = qnam.get(QNetworkRequest(QUrl(imgurl)));
    connect(imgreply, SIGNAL(finished()), this, SLOT(imgFinished()));
    connect(imgreply, SIGNAL(readyRead()), this, SLOT(imgReadyRead()));
    return 0;
}

void AlbumArtGetter::imgFinished()
{
    imagefile->close();
    qDebug() << "Downloaded file " + filename;
}

void AlbumArtGetter::imgReadyRead()
{
    if(imagefile)
        imagefile->write(imgreply->readAll());
}
