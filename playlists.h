#ifndef PLAYLISTS_H
#define PLAYLISTS_H

#include <QtGui>

class Playlists : public QWidget
{
    Q_OBJECT

public:
    Playlists();
    QWidget* makeSidebar(QString path);
    void setPath(char* path);

private:

    QWidget* sidewidget;
    QGridLayout* sidebarlayout;
    //properties and method for each section of screen

    QPushButton* playlistbtn;
    QPushButton* makePlaylistBtn();

    QWidget* playlistbar;
    QWidget* previewbar;
    QWidget* makePlaylistBar();

    QPushButton* previewbtn;
    QPushButton* makePreviewBtn();

    QString expath;
};

#endif // PLAYLISTS_H
