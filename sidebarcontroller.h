#ifndef PLAYLISTS_H
#define PLAYLISTS_H

#include "utilities.h"
#include "database.h"
#include "playlistscontext.h"

class LibraryController;
class PreviewPane;

class SidebarController : public QObject
{
    Q_OBJECT
public:
    SidebarController(Utilities& utilities, Database& datab, LibraryController* lib);
    ~SidebarController();
    QWidget* getWidget();
    QWidget* makeWidget();
    void updateplaylistbar(int i);

signals:
  void playthis(QList<QSqlRecord>*);
  void displayAllMedia();
  void customContextMenuRequested ( const QPoint & pos  );
  void playsmartplaylist(QString);
  void playplaylist(QString);
  void rolledover(QString);
  void rolldefault();

  public slots:
    void Clicked(int row, int column);
    void DoubleClicked(int row, int column);
    void ShowContextMenu(const QPoint&);
    void setSelectedPlaylist(QString);
    void RolloverCell(int,int);


private:
    QWidget* widget;
    QWidget* makePlaylistRow(QString name, QWidget* parent);
    QWidget* main;
    int dontview;

    QTableWidget* playlistbar;
    QWidget* previewbar;
    QWidget* playlistbtn;
    QWidget* previewbtn;

    QGridLayout* sidebarlayout;
    QTableWidget* playlistTableWidget;

    QWidget* makePlaylistBtn();
    QWidget* makePreviewBtn();
    QWidget* makePreviewBar();
    QTableWidget* buildButtons();
    PreviewPane* preview;


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
