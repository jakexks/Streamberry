#ifndef LIBRARYCONTROLLER_H
#define LIBRARYCONTROLLER_H

#include "utilities.h"
#include "albumartdelegate.h"
#include "trackcontext.h"
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
    void setHeaders(QStringList& headers, int sortcol);

signals:
    void songInfoData(QString album, QString artist, QString title, QString track);

public slots:
    void sortLibrary();
    void deselectFirst();
    void sortIndicatorChanged(int index,Qt::SortOrder order);
    void sectionResized(int logicalindex, int oldsize, int newsize);
    void itemClicked(int row);
    void playNextFile();
    void playPrevFile();
    void displaythis(QList<QSqlRecord>*);
    void setSearchText(QString text);
    void updateLibrary();
    void musicVideoFilter(int value);
    void ShowContextMenu(const QPoint& pos);
    //void DragStart(QTableWidgetItem*);

private:
    Utilities& util;
    //void ActualDrag(QTableWidgetItem* item, const QPoint startPos);
    QWidget* widget;
    QWidget* curview;
    QGridLayout* container;
    int headercount;
    int sortcolumn;
    int currentlyplaying;
    int musicvideofilter;
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
    TrackContext* trackmenu;
};

#endif // LIBRARYCONTROLLER_H
