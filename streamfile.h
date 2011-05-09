#ifndef STREAMFILE_H
#define STREAMFILE_H

#include <vlc/vlc.h>
#include <QString>
#include <QObject>
#include <QHash>
#include <QFileInfo>
#include "streamrequest.h"
#include "player.h"
#include "networking.h"

//Controls the streaming of files
class StreamFile : public QObject
{
    Q_OBJECT;
public:
    StreamFile(Player& _player);
    ~StreamFile();

    void addStream(QString fileName, QString compID, QString ipAddress);
    void removeStream(QString compID);
    void changeStream(QString compID, QString newFileName);
    void pauseStream(QString compID);
    void stopStream(QString compID);
    void seekStream(QString compID, float percentage);
    float getStreamPosition(QString compID);
    int getStreamTime(QString compID);
    //Returns the length of the stream with given UID
    int getStreamLength(QString compID);
    bool isSameStream(QString compID, QString filepath);

public slots:
    //Start playing the strean
    void playStream(QString compID);
private:
    libvlc_instance_t *_vlcinstance;
    StreamRequest stream;
    Player& player;
    QHash<QString, int> ispaused;
    networking n;

private slots:
    //Parse the message coming in from streamRequest class
    void parseMessage(QString message);
};

#endif // STREAMFILE_H
