#ifndef STREAMFILE_H
#define STREAMFILE_H

#include <vlc/vlc.h>
#include <QString>
#include <QObject>

#include "streamrequest.h"

class StreamFile : public QObject
{
    Q_OBJECT;
public:
    StreamFile();
    ~StreamFile();

    void addStream(QString fileName, QString compID, QString ipAddress);
    void removeStream(QString compID);
    void changeStream(QString compID, QString newFileName);
    void pauseStream(QString compID);
    void stopStream(QString compID);
    void seekStream(QString compID, float percentage);
    float getStreamPosition(QString compID);
    int getStreamTime(QString compID);
    int getStreamLength(QString compID);
    bool isSameStream(QString compID, QString filepath);

public slots:
    void playStream(QString compID);
private:
    libvlc_instance_t *_vlcinstance;
    StreamRequest stream;

private slots:
    void parseMessage(QString message);
};

#endif // STREAMFILE_H
