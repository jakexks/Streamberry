#ifndef PREVIEWPANE_H
#define PREVIEWPANE_H
#include "utilities.h"
#include "database.h"

class Utilities;
class Database;
class LibraryController;
class QStackedWidget;
class PreviewPane : public QObject
{
    Q_OBJECT
public:
    PreviewPane(Utilities &utilities, Database& datab, LibraryController* lib);
    ~PreviewPane();
    QWidget* getwidget();

  public slots:
    void rolloverPlaylist(QString);
    void displayAlbumArt(QImage);
    void rolloverDefault();

private:
    Utilities* util;
    QWidget* makeDefault();
    QWidget* makePlaylist();
    QWidget* makeArt();
    void updateArt(QImage pic);
    void updatePreview(QString name);
    Database* db;
    LibraryController* libcont;
    QWidget* mainwidget;
    int state;
    QStackedWidget* window;
};

#endif // PREVIEWPANE_H
