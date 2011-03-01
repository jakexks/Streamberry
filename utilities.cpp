#include "utilities.h"
#include "crossplatform.h"
#include <QDateTime>
#include <QString>
#include <QDebug>

Utilities::Utilities(char* exec)
{
    execpath = QString(exec);
    execpath.resize(execpath.lastIndexOf('/'));
    execpath += "/";
    stylesheet = makeStylesheet();
}

int Utilities::getCurrentTimestamp()
{
    return QDateTime::fromTime_t(0).secsTo(QDateTime::currentDateTime());
}

QString Utilities::getExecutePath()
{
    return execpath;
}

QString Utilities::makeStylesheet()
{

    QString style = "#topBarBack {background: url(EXECPATHimages/topBarBackUp.png) no-repeat; border: none; padding:0;}";
    style+="#topBarForward {background: url(EXECPATHimages/topBarForwardUp.png) no-repeat;border: none; padding:0;}";
    style+="#topBarMusic{background: url(EXECPATHimages/topBarMusicVideoUp.png) no-repeat;border: none; padding:0;}";
    style+="#topBarWidget{background:url(EXECPATHimages/topBarBackground.png); border: none; padding:0px;}";
    style.replace("EXECPATH", execpath);
    qDebug()<<execpath<<" "<<style;
    return style;
}

QString Utilities::getStylesheet()
{
    return stylesheet;
}
