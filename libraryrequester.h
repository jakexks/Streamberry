#ifndef LIBRARYREQUESTER_H
#define LIBRARYREQUESTER_H
#include <QThread>

class LibraryRequester : public QThread
{
    Q_OBJECT
public:
    LibraryRequester();
public slots:
    //void getLibrary(QString theirid, QString dblastupdate);
};

#endif // LIBRARYREQUESTER_H
