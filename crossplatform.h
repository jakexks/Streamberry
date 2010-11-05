#ifndef CROSSPLATFORM_H
#define CROSSPLATFORM_H

#include <QString>

//Class will be for methods that resolve differences between platforms.

class CrossPlatform
{
public:
    CrossPlatform();
    QString static getAppDataPath();
};

#endif // CROSSPLATFORM_H
