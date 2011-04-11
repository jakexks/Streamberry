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
    void makeWidget();
    void setHeaders(QStringList& headers, int sortcol);
    void pushNormalPlaylist(QString name);
    void pushSmartPlaylist(QString name, QString filtertext);
    void pushAllView();
    int getCurrentViewType();
    QString getCurrentPlaylistName();
signals:
    void songInfoData(QString album, QString artist, QString title, QString track);
    void setSearchBoxText(QString text);
    void setSelectedPlaylist(QString name);
public slots:
    void sortLibrary();
    int rowToHighlight();
    void playplaylist(QString playlistname);
    void playsmartplaylist(QString playlistname);
    void deselectFirst();
    void sortIndicatorChanged(int index,Qt::SortOrder order);
    void sectionResized(int logicalindex, int oldsize, int newsize);
    void itemClicked(int row);
    void playNextFile();
    void playPrevFile();
    void setSearchText(QString text);
    void updateLibrary();
    void musicVideoFilter(int value);
    void ShowContextMenu(const QPoint& pos);
    void goBack();
    void goForward();
    //void DragStart(QTableWidgetItem*);

private:
    void fillData(QList<QSqlRecord>* values);
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
    QList<QSqlRecord>* playingdata;
    AlbumArtDelegate* paneldelegate;
    Database& db;
    Player& player;
    QString searchtext;
    QString smarttext;
    QString playlist;
    TrackContext* trackmenu;
    typedef struct {
        QString playlist;
        QString playlisttitle;
        QString searchtext;
        QString smarttext;
        QList<QString> sortcols;
        QList<QString> orders;
    } ViewQueueItem;
    int viewqueueindex;
    QList<ViewQueueItem> viewqueue;
};

#endif // LIBRARYCONTROLLER_H
