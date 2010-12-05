/* libVLC and Qt sample code
 * Copyright © 2009 Alexander Maringer <maringer@maringer-it.de>
 */

#include "vlc_on_qt.h"
#include "file_meta.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{

    //File_Meta a;
    //a.print_meta("./01-Metric-Help I'm Alive.mp3"); //Replace with file name to print Title
    QApplication a(argc, argv);
    Player p;
    p.resize(640,480);
    p.playFile("./k_k13fo4pr4.flv"); // Replace with what you want to play
    p.show();
    return a.exec();
}
