#include "streamfile.h"

StreamFile::StreamFile()
{
    const char * vlc_args[] = {
                        "--ignore-config",
                        "--no-plugins-cache",
                        "--verbose=0"};

    //const char * sout = "#duplicate{dst=display,dst=rtp{mux=ts,dst=127.0.0.1}}";
    _vlcinstance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

    stream.startServer();
    connect(&stream, SIGNAL(messageReceived(QString)), this, SLOT(parseMessage(QString)));
}

void StreamFile::addStream(QString fileName, QString compID, QString ipAddress)
{
    QString sout = "#rtp{mux=ts,dst=";
    sout += ipAddress.toUtf8();
    sout += "}}";
    libvlc_vlm_add_broadcast(_vlcinstance, compID.toUtf8(), fileName.toUtf8(),
                                sout.toUtf8(), 0, NULL, TRUE, TRUE);
    libvlc_vlm_play_media(_vlcinstance, compID.toUtf8());

}

void StreamFile::removeStream(QString compID)
{
    libvlc_vlm_del_media(_vlcinstance, compID.toUtf8());
}

void StreamFile::changeStream(QString compID, QString newFileName)
{
    libvlc_vlm_set_input(_vlcinstance, compID.toAscii(), newFileName.toUtf8());
}

void StreamFile::playStream(QString compID)
{
    libvlc_vlm_play_media(_vlcinstance, compID.toUtf8());
}

void StreamFile::pauseStream(QString compID)
{
    libvlc_vlm_pause_media(_vlcinstance, compID.toUtf8());
}

void StreamFile::stopStream(QString compID)
{
    libvlc_vlm_stop_media(_vlcinstance, compID.toUtf8());
}

void StreamFile::seekStream(QString compID, float percentage)
{
    libvlc_vlm_seek_media(_vlcinstance, compID.toUtf8(), percentage);
}

float StreamFile::getStreamPosition(QString compID)
{
    return libvlc_vlm_get_media_instance_position(_vlcinstance, compID.toUtf8(), 0);
}

int StreamFile::getStreamTime(QString compID)
{
    return libvlc_vlm_get_media_instance_time(_vlcinstance, compID.toUtf8(), 0)/1000000;
}

int StreamFile::getStreamLength(QString compID)
{
    int length = 0;
    while(length < 1 && length != -1){
        length = libvlc_vlm_get_media_instance_length(_vlcinstance, compID.toUtf8(), 0)/1000000;
    }
    return length;
}

void StreamFile::parseMessage(QString message)
{
    QStringList split = message.split('|', QString::KeepEmptyParts, Qt::CaseInsensitive);
    QString uniqueID = split.at(3);
    QString ipaddress = split.at(2);
    QString filepath = split.at(4);
    if(split.at(1)=="PLAY")
    {
        if(libvlc_vlm_show_media(_vlcinstance, uniqueID.toAscii())!=NULL)
        {
            stopStream(uniqueID);
            removeStream(uniqueID);
            //sleep(2);
        }
        addStream(filepath, uniqueID, ipaddress);
    } else if (split.at(1)=="STOP") {
        stopStream(uniqueID);
    }
    //qDebug() << message;
}

StreamFile::~StreamFile()
{
    libvlc_vlm_release(_vlcinstance);
}
