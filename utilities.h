#ifndef UTILITIES_H
#define UTILITIES_H
#include <QString>

class Utilities
{
public:
    Utilities(char* exec);
    //get timestamp (represented by seconds since 1am 1970)
    int static getCurrentTimestamp();
    QString getExecutePath();
    QString makeStylesheet();
    QString getStylesheet();
    QString getColor(int inputVar);

private:
    QString execpath;
    QString stylesheet;
};

#endif // UTILITIES_H
