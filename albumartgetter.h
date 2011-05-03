#ifndef ALBUMARTGETTER_H
#define ALBUMARTGETTER_H
#include <QtNetwork>
#include <QUrl>
#include <QFile>

class AlbumArtGetter : public QObject
{
    Q_OBJECT
public:
    AlbumArtGetter();
    int get(QString Artist, QString Album);
private slots:
    void httpFinished();
    void httpReadyRead();
    void imgFinished();
    void imgReadyRead();
private:
    QNetworkAccessManager qnam;
    QNetworkReply* reply;
    QNetworkReply* imgreply;
    QString LAST_API_KEY;
    QString received;
    QString filename;
    QFile *imagefile;
    int downloadImage(QString url);
};

#endif // ALBUMARTGETTER_H
