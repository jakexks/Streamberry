#ifndef PREVIEWPANE_H
#define PREVIEWPANE_H
#include "utilities.h"
#include "database.h"

class Utilities;
class Database;
class LibraryController;
class QStackedWidget;
class QTableWidget;
class QLabel;

//The preview pane in the sidebar
class PreviewPane : public QObject
{
    Q_OBJECT
public:
  //instantiates the preview pane, sets up it's connection to the database and the libraryController
    PreviewPane(Utilities &utilities, Database& datab, LibraryController* lib);
    ~PreviewPane();
    QWidget* getwidget();

  public slots:
    //These slots below are recieved by the preview pane when something is rolled over and the content should change
    void rolloverPlaylist(QString);
    void displayAlbumArt();
    void displayAlbumArt(QPixmap);
    void rolloverDefault();
    void setDefault();
    //These two slots recieve signals from the player allowing the timer to be updated.
    void settracklength(int seconds);
    void settrackprogress(float pos);

private:
    //Holds a pointer to the utilities class
    Utilities* util;
    //Create the time banner that runs along the bottom of the preview pane
    QFrame* maketimebar();
    //Create the default rollover view
    QWidget* makeDefault();
    //create the table used for a playlist rollover
    QWidget* makePlaylist();
    //create the frame used when a song is playing and art is displayed
    QWidget* makeArt();
    //Updates the art panel to contain new art for the rollover
    void updateArt(QPixmap pic);
   //updates the playlist preview with the tracks from the currently hovered playlist
    void updatePreview(QString name);
    //Holds a pointer to the database
    Database* db;
    //holds a pointer to the library controller
    LibraryController* libcont;
    //holds a pointer to this widget
    QWidget* mainwidget;
    int state;
    QStackedWidget* window;
    QLabel* pictureframe;
    QTableWidget* tableframe;
    QLabel* timetext;
    QFrame* timebar;
    int showing;
    int timelength;
    int hold;
    int finalseconds;
  };

#endif // PREVIEWPANE_H
