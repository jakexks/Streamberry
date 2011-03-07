#ifndef PLAYLISTS_H
#define PLAYLISTS_H

#include "utilities.h"
#include "database.h"
#include "playlistscontext.h"

class LibraryController;

class SidebarController : public QWidget
{
    Q_OBJECT

public:
    SidebarController(Utilities& utilities, Database& datab, LibraryController& lib);
    QWidget* getWidget();
    QWidget* makeWidget();
    void updateplaylistbar(int i);

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
    QTableWidget* topButtons;
    QGridLayout* sidebarlayout;
    //QGridLayout* playlistbarlayout;
    QTableWidget* displayTableWidget;
    QTableWidget* playlistTableWidget;

    QWidget* makePlaylistBtn();
    QWidget* makePreviewBtn();
    QWidget* makePreviewBar();
    QTableWidget* buildButtons();


    QTableWidget* buildplaylistbar();


    //QTableWidget* makePlaylistBar();

    Utilities& util;
    QString expath;

    PlaylistMenu* smartmenu;
    PlaylistMenu* normalmenu;

    bool typearray[15];
    Database& db;
    LibraryController& libpass;
};

#endif // PLAYLISTS_H
