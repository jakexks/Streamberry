#include "streamfile.h"

StreamFile::StreamFile(Player& _player) : player(_player)
{
    const char * vlc_args[] = {
                        "--ignore-config",
                        "--no-plugins-cache",
                        "--verbose=0",
                        "--ts-out-mtu=10",
                        "--sout-rtp-caching=10",
                        "--sout-keep",
                    };

    //const char * sout = "#duplicate{dst=display,dst=rtp{mux=ts,dst=127.0.0.1}}";
    _vlcinstance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

    stream.startServer();
    connect(&stream, SIGNAL(messageReceived(QString)), this, SLOT(parseMessage(QString)));
}

void StreamFile::addStream(QString fileName, QString compID, QString ipAddress)
{
    QString sout = "";
    QFileInfo file(fileName);
    if ( !file.exists() ) return;//If file doesn't exist, return.
    if (file.suffix()=="wma")
    {
        sout = "#transcode{vcodec=none,acodec=mpga,ab=128,channels=2,samplerate=44100}:rtp{mux=ts,dst=";
    }
    else {
        sout = "#rtp{mux=ts,dst=";
    }
    sout += ipAddress.toUtf8();
    sout += "}}";
    libvlc_vlm_add_broadcast(_vlcinstance, compID.toAscii(), fileName.toUtf8(),
                                sout.toUtf8(), 0, NULL, TRUE, FALSE);
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
    //If playing something, then pause
    if(ispaused.value(compID)==0)
    {
        int pos = getStreamPosition(compID)*10000;
        libvlc_vlm_stop_media(_vlcinstance, compID.toAscii());
        qDebug() << pos;
        ispaused[compID] = pos;
    }
    else
    {
        qDebug() << ispaused.value(compID);
        //libvlc_vlm_stop_media(_vlcinstance, compID.toAscii());

        playStream(compID);
        seekStream(compID, (float)ispaused.value(compID)/10000);
        ispaused[compID] = 0;
    }
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

bool StreamFile::isSameStream(QString compID, QString filepath)
{
    QString temp = libvlc_vlm_show_media(_vlcinstance, compID.toAscii());
    temp = temp.mid((temp.indexOf('[')+5));
    temp.resize(filepath.length());
    if(temp == filepath)
    {
        return true;
    } else {
        return false;
    }
}

void StreamFile::parseMessage(QString message)
{
    QStringList split = message.split('|', QString::KeepEmptyParts, Qt::CaseInsensitive);

    qDebug() << "RECEIVED IN STREAMER" << message;

    if(split.at(1)=="PLAY")
    {
        QString uniqueID = split.at(3);
        QString ipaddress = split.at(2);
        QString filepath = split.at(4);
        QString intendedipaddress = split.at(5);
        if (intendedipaddress != n.getmyip())
        {
            return;
        }
        //Check if something is playing
        if(libvlc_vlm_show_media(_vlcinstance, uniqueID.toAscii())!=NULL)
        {
            if(isSameStream(uniqueID.toAscii(), filepath)) {
                stopStream(uniqueID.toAscii());
                removeStream(uniqueID.toAscii());
                addStream(filepath, uniqueID, ipaddress);

            } else {
                stopStream(uniqueID.toAscii());
                removeStream(uniqueID.toAscii());
                addStream(filepath, uniqueID, ipaddress);
            }
            ispaused[uniqueID] = 0;
            /*qDebug() << libvlc_vlm_show_media(_vlcinstance, uniqueID.toAscii());
            stopStream(uniqueID);
            removeStream(uniqueID);
            qDebug() << libvlc_vlm_show_media(_vlcinstance, uniqueID.toAscii());
            sleep(5);
            qDebug() << libvlc_vlm_show_media(_vlcinstance, uniqueID.toAscii());*/
            //sleep(2);
        } else {
            ispaused[uniqueID] = 0;
            addStream(filepath, uniqueID, ipaddress);
        }
        QString tosend = "";
        tosend += "STREAMBERRY|LENGTH|";
        tosend += QString::number(getStreamLength(uniqueID));
        stream.send(ipaddress, 45459, tosend);
    } else if (split.at(1)=="STOP") {
        stopStream(split.at(2));
    } else if (split.at(1)=="PAUSE") {
        pauseStream(split.at(2));
    } else if (split.at(1)=="SEEK") {
        seekStream(split.at(2), split.at(3).toFloat());

    } else if (split.at(1)=="LENGTH") {
        player.setFileLength(split.at(2).toInt());
    }
    //qDebug() << message;
}

StreamFile::~StreamFile()
{
    libvlc_vlm_release(_vlcinstance);
}
