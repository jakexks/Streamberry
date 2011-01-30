#ifndef PLAYLISTS_H
#define PLAYLISTS_H

#include <QtGui>

class playlists : public QWidget
{
    Q_OBJECT

public:
    playlists();
    QWidget* makesidebar(QString path);
    void setPath(char* path);

private:

    QWidget* SideWidget;
    QGridLayout* SideBarLayout;
    //properties and method for each section of screen

    QPushButton* PlaylistBtn;
    QPushButton* makePlaylistBtn();

    QWidget* PlaylistBar;
    QWidget* PreviewBar;
    QWidget* makePlaylistBar();

    QPushButton* PreviewBtn;
    QPushButton* makePreviewBtn();

    QString expath;
};

#endif // PLAYLISTS_H
