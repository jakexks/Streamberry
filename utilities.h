#ifndef UTILITIES_H
#define UTILITIES_H
#include <QString>

class Utilities
{
public:
    Utilities(QString exec);
    //get timestamp (represented by seconds since 1am 1970)
    int static getCurrentTimestamp();
    QString getExecutePath();
    QString getStylesheet();
    QString getColor(int inputVar);
    static QString intToTime(int seconds);

    static int getCount();
    static void incCount();
    static void setCount(int newc);



private:
    QString makeStylesheet();
    QString execpath;
    QString stylesheet;
    static int PLcount;
};

#endif // UTILITIES_H
