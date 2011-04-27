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

    void settracklength(int seconds);
    void settrackprogress(int seconds);

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
    int showing;
    int timelength;
  };

#endif // PREVIEWPANE_H
