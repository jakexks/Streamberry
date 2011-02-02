#ifndef PLAYLISTS_H
#define PLAYLISTS_H

#include "utilities.h"
#include <QtGui>

class SidebarController : public QWidget
{
    Q_OBJECT

public:
    SidebarController(Utilities& utilities);
    QWidget* getWidget();
    QWidget* makeWidget();

private:
    QPushButton* makePlaylistBtn();
    QPushButton* makePreviewBtn();
    QWidget* makePlaylistBar();

    QPushButton* playlistbtn;
    QWidget* playlistbar;
    QWidget* previewbar;
    QGridLayout* sidebarlayout;
    QPushButton* previewbtn;

    Utilities& util;
    QString expath;
    QWidget* widget;
};

#endif // PLAYLISTS_H
