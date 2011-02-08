#ifndef LIBRARYCONTROLLER_H
#define LIBRARYCONTROLLER_H

#include "utilities.h"
#include "albumartdelegate.h"
#include "database.h"
#include <QtGui>
#include <QSqlRecord>
#include <QList>
#include <QObject>

class LibraryController : public QObject
{
    Q_OBJECT

public:
    LibraryController(Utilities& utilities, Database& datab);
    ~LibraryController();
    QWidget* getWidget();
    void addHeaders();
    void fillData(QList<QSqlRecord>* values);
    void makeWidget();
    void setHeaders(QList<QString>& headers, int sortcol);
signals:
    void needNewLibrary(QList<QString>* sortcols, QList<QString>* order);
public slots:
    void deselectFirst();
    void sortIndicatorChanged(int index,Qt::SortOrder order);
    void sectionResized(int logicalindex, int oldsize, int newsize);
private:
    Utilities& util;
    QWidget* widget;
    QWidget* curview;
    QGridLayout* container;
    int headercount;
    int sortcolumn;
    Qt::SortOrder sortorder;
    QString** curheaders;
    QTableWidget* tablewidget;
    QList<QSqlRecord>* currentdata;
    AlbumArtDelegate* paneldelegate;
    Database& db;
};

#endif // LIBRARYCONTROLLER_H
