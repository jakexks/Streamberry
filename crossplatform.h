#ifndef CROSSPLATFORM_H
#define CROSSPLATFORM_H

#include "utilities.h"
#include <QString>

//Class for methods that resolve differences between platforms.

class CrossPlatform
{
public:
    CrossPlatform();
    QString static getAppDataPath();
    QString static getImageDirPath(Utilities *util);
};

#endif // CROSSPLATFORM_H
