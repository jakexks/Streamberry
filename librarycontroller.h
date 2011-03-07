#ifndef LIBRARYCONTROLLER_H
#define LIBRARYCONTROLLER_H

#include "utilities.h"
#include "albumartdelegate.h"
#include "database.h"
#include <sbsearchbar.h>
#include <QtGui>
#include <QSqlRecord>
#include <QList>
#include <QObject>
class Player;


class LibraryController : public QObject
{
    Q_OBJECT

public:
    LibraryController(Utilities& utilities, Database& datab, Player& p, SBSearchBar* searchbar);
    ~LibraryController();
    QWidget* getWidget();
    void addHeaders();
    void fillData(QList<QSqlRecord>* values);
    void makeWidget();
    void setHeaders(QList<QString>& headers, int sortcol);
signals:
    void needNewLibrary();
public slots:
    void sortLibrary();
    void deselectFirst();
    void sortIndicatorChanged(int index,Qt::SortOrder order);
    void sectionResized(int logicalindex, int oldsize, int newsize);
    void itemClicked(int row, int column);
    void playNextFile();
    void playPrevFile();
    void setSearchText(QString text);
private:
    Utilities& util;
    QWidget* widget;
    QWidget* curview;
    QGridLayout* container;
    int headercount;
    int sortcolumn;
    int currentlyplaying;
    Qt::SortOrder sortorder;
    QString** curheaders;
    QTableWidget* tablewidget;
    QList<QSqlRecord>* currentdata;
    AlbumArtDelegate* paneldelegate;
    Database& db;
    Player& player;
    QString searchtext;
    QList<QString> *sortcols;
    QList<QString> *orders;
    void updateLibrary();
};

#endif // LIBRARYCONTROLLER_H
