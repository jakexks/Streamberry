#ifndef CROSSPLATFORM_H
#define CROSSPLATFORM_H

#include "utilities.h"
#include <QString>

//Class for methods that resolve differences between platforms.
class CrossPlatform
{
public:
    CrossPlatform();
    //returns the path of the place you can store data on the system
    QString static getAppDataPath();
    //path of the image folder
    QString static getImageDirPath(Utilities *util);
};

#endif // CROSSPLATFORM_H
