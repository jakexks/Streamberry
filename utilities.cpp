#include "utilities.h"
#include "crossplatform.h"
#include <QDateTime>
#include <QString>

Utilities::Utilities(char* exec): execpath(QString(exec))
{
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
    QString style = "";
    return style;
}

QString Utilities::getStylesheet()
{
    return stylesheet;
}
