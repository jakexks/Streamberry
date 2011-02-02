#ifndef LIBRARYCONTROLLER_H
#define LIBRARYCONTROLLER_H

#include "utilities.h"
#include <QtGui>

class LibraryController
{
public:
    LibraryController(Utilities& utilities);
    QWidget* getWidget();
    QWidget* makeWidget();
private:
    Utilities& util;
    QWidget* widget;
};

#endif // LIBRARYCONTROLLER_H
