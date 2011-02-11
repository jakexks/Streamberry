#ifndef STREAMFILE_H
#define STREAMFILE_H

#include <vlc/vlc.h>
#include <QString>

class StreamFile
{
public:
    StreamFile();
    ~StreamFile();

    void addStream(QString fileName, QString compID, QString ipAddress);
    void removeStream(QString compID);
    void changeStream(QString compID, QString newFileName);
    void playStream(QString compID);
    void pauseStream(QString compID);
    void stopStream(QString compID);
    void seekStream(QString compID, float percentage);
    float getStreamPosition(QString compID);
    int getStreamTime(QString compID);
    int getStreamLength(QString compID);

private:
    libvlc_instance_t *_vlcinstance;
};

#endif // STREAMFILE_H
