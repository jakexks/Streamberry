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

//controls the library view
class LibraryController : public QObject
{
    Q_OBJECT

public:
    LibraryController(Utilities& utilities, Database& datab, Player& p, SBSearchBar* searchbar);
    ~LibraryController();
    //returns the library widget
    QWidget* getWidget();
    //add the headers to the library view
    void addHeaders();
    //make the library widget
    void makeWidget();
    //set the header names
    void setHeaders(QStringList& headers, int sortcol);
    //add a normal playlist to the library view list
    void pushNormalPlaylist(QString name);
    //add a smart/filtered playlist to the library view list
    void pushSmartPlaylist(QString name, QString filtertext);
    //add a view showing everything to the library view list
    void pushAllView();
    //add the video view to the library view list
    void pushVideoView();
    //get the view currently showing
    int getCurrentViewType();
    //get the name of the playlist currently being shown
    QString getCurrentPlaylistName();
signals:
    //connected to the song info panel
    void songInfoData(QString album, QString artist, QString title, QString track);
    //update the search box text
    void setSearchBoxText(QString text);
    //highlight a playlist in the sidebar
    void setSelectedPlaylist(QString name);
    //pause the player
    void pausePlayer();
    //show the default streamberry logo in the preview pane
    void rolldefault();
    //show the album art cover in the preview pane
    void rollAlbum();
    //select the video item from the sidebar
    void selectVideo();

public slots:
    //what you should call when sorting order has been changed
    void sortLibrary();
    //highlight a playing row
    int rowToHighlight();
    void playplaylist(QString playlistname);
    void playsmartplaylist(QString playlistname);
    void deselectFirst();
    //update library content to match new sort order
    void sortIndicatorChanged(int index,Qt::SortOrder order);
    //Store changes in header size
    void sectionResized(int logicalindex, int oldsize, int newsize);
    //item clicked so play it
    void itemClicked(int row);
    //play next item in playing list
    void playNextFile();
    //play previous item in playing list
    void playPrevFile();
    //push to the view list the text in the search
    void setSearchText(QString text);
    //pulls a new copy of the stuff to view from the database
    void updateLibrary();
    //filters by music/video. nothing=2, music=0, video=1
    void musicVideoFilter(int value);

    //show context menu for a mouse position
    void ShowContextMenu(const QPoint& pos);
    //move back through view list
    void goBack();
    //move forward through view list
    void goForward();
    //toggle shuffle
    void shuffleSlot();
    //turn repeat on or off
    void repeatSlot(bool one, bool all);
    //empty view list
    void resetQueue();
    //say a table cell is rolled over
    void cellrolled(int, int);
    //void DragStart(QTableWidgetItem*);

private:
    //fill data in the table with the items in the list
    void fillData(QList<QSqlRecord>* values);
    //generate random number between values
    int randInt(int low, int high);
    Utilities& util;
    //void ActualDrag(QTableWidgetItem* item, const QPoint startPos);
    //outer widget of all views
    QWidget* widget;
    //the currently displayed view
    QWidget* curview;
    //container to easily swap out views
    QGridLayout* container;
    //stores all views available
    QStackedWidget* allwidgets;
    //number of headers
    int headercount;
    //column being sorted by (deprecated)
    int sortcolumn;
    //currently playing row
    int currentlyplaying;
    //how the current view is filtered (deprecated)
    int musicvideofilter;
    //whether shuffle is on or not
    int shuffle;
    int numberiterator;
    //repeat state
    int repeat;
    //ordering of indexes of currently playing
    int *shufflelist;
    int maxsize;
    //sort order of header
    Qt::SortOrder sortorder;
    //the current headers in view
    QString** curheaders;
    //library widget
    QTableWidget* tablewidget;
    //the data currently being viewed
    QList<QSqlRecord>* currentdata;
    //the data currently being played
    QList<QSqlRecord>* playingdata;
    //creates album panels when they are shown in the table
    AlbumArtDelegate* paneldelegate;
    Database& db;
    Player& player;
    //current search text (deprecated)
    QString searchtext;
    //text given by smart playlist
    QString smarttext;
    QString playlist;
    //stores track context menu
    TrackContext* trackmenu;
    //the structure that the view list holds. properties track state of view.
    typedef struct {
        //playlist being viewed
        QString playlist;
        //playlist title
        QString playlisttitle;
        //text in the search box
        QString searchtext;
        //text given by smart playlist
        QString smarttext;
        //whether the video is being shown
        int videoview;
        //column sorting by
        QList<QString> sortcols;
        //order being sorted by
        QList<QString> orders;
    } ViewQueueItem;
    //index in view list being currently viewed
    int viewqueueindex;
    //stores the view list
    QList<ViewQueueItem> viewqueue;
    //shuffles currently playing and stores permutation
    void makeShuffleList(int firstsong);
};

#endif // LIBRARYCONTROLLER_H
