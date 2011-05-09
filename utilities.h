#ifndef UTILITIES_H
#define UTILITIES_H
#include <QString>

//All generic utilities that are useful to more than one class
class Utilities
{
public:
    Utilities(QString exec);
    //get timestamp (represented by seconds since 1am 1970)
    int static getCurrentTimestamp();
    //get the path of the executable
    QString getExecutePath();
    //get the stylesheet for the GUI
    QString getStylesheet();
    //specifies the colors of the background of the library rows
    QString getColor(int inputVar);
    //convert an integer in seconds to a string minutes:seconds representation
    static QString intToTime(int seconds);

    //controls playlist counts
    static int getCount();
    static void incCount();
    static void setCount(int newc);

private:
    //generate the stylesheet string
    QString makeStylesheet();
    //stores executable page
    QString execpath;
    //stores stylesheet string
    QString stylesheet;
    //stores number of playlists
    static int PLcount;
};

#endif // UTILITIES_H
