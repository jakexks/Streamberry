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
    QWidget* makePlaylistBtn();
    QWidget* makePreviewBtn();
    QWidget* makePreviewBar();
    QWidget* makePlaylistBar();

    QWidget* playlistbar;
    QWidget* previewbar;
    QWidget* playlistbtn;
    QWidget* previewbtn;

    QTableWidget* topButtons;

    QTableWidget* buildButtons();
    QGridLayout* sidebarlayout;
    QGridLayout* playlistbarlayout;
    QTableWidget* tablewidget;

    void buildplaylistbar(QWidget*);
    Utilities& util;
    QString expath;
    QWidget* widget;
    PlaylistMenu* smartmenu;
    PlaylistMenu* normalmenu;

    bool typearray[15];
    Database& db;
    LibraryController& libpass;
};

#endif // PLAYLISTS_H
