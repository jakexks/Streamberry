#ifndef PLAYLISTS_H
#define PLAYLISTS_H

#include "utilities.h"
#include "database.h"
#include "playlistscontext.h"

class LibraryController;

class SidebarController : public QObject
{
    Q_OBJECT
public:
    SidebarController(Utilities& utilities, Database& datab, LibraryController* lib);
    QWidget* getWidget();
    QWidget* makeWidget();
    void updateplaylistbar(int i);

signals:
  void playthis(QList<QSqlRecord>*);
  void displayAllMedia();

  public slots:
    void Clicked(int row, int column);
    void DoubleClicked(int row, int column);
    void ShowContextMenu(const QPoint&);

signals:
    void customContextMenuRequested ( const QPoint & pos  );


private:
    QWidget* widget;
    QWidget* makePlaylistRow(QString name);
    QWidget* main;

    QTableWidget* playlistbar;
    QWidget* previewbar;
    QWidget* playlistbtn;
    QWidget* previewbtn;
    QLabel* timetext;
    QGridLayout* sidebarlayout;
    QTableWidget* playlistTableWidget;

    QWidget* makePlaylistBtn();
    QWidget* makePreviewBtn();
    QWidget* makePreviewBar();
    QTableWidget* buildButtons();


    QTableWidget* buildplaylistbar();

    Utilities& util;

    PlaylistMenu* smartmenu;
    PlaylistMenu* normalmenu;

    bool typearray[15];
    QString namearray[15];

    Database& db;
    LibraryController* libpass;
};

#endif // PLAYLISTS_H
