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
    PreviewPane(Utilities &utilities, Database& datab, LibraryController* lib);
    ~PreviewPane();
    QWidget* getwidget();

  public slots:
    void rolloverPlaylist(QString);
    void displayAlbumArt();
    void displayAlbumArt(QPixmap);
    void rolloverDefault();
    void setDefault();
    void settracklength(int seconds);
    void settrackprogress(float pos);

private:
    Utilities* util;
    QFrame* maketimebar();
    QWidget* makeDefault();
    QWidget* makePlaylist();
    QWidget* makeArt();
    void updateArt(QPixmap pic);
    void updatePreview(QString name);
    Database* db;
    LibraryController* libcont;
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
