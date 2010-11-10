#include "crossplatform.h"
#include <QString>
#include <QDir>

CrossPlatform::CrossPlatform() {}

//Because application data path is platform specific, need this function.
QString CrossPlatform::getAppDataPath()
{
    #ifdef Q_WS_WIN
    return QDir::homePath() + "/Application Data/Streamberry/";
    #endif

    #ifdef Q_WS_MAC
    return QDir::homePath() + "/Library/Application Support/Streamberry/";
    #endif

    #ifdef Q_WS_X11
    return QDir::homePath() + "/.streamberry/";
    #endif

    return "/";
}
