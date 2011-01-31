#include "utilities.h"
#include <QDateTime>

int Utilities::getCurrentTimestamp()
{
    return QDateTime::fromTime_t(0).secsTo(QDateTime::currentDateTime());
}
