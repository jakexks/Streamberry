#ifndef SONGINFO_H
#define SONGINFO_H
#include <QtGui>
#include <QObject>
#include "utilities.h"

//The song info widget in bottom bar
class SongInfo: public QObject
{
    Q_OBJECT;
public:
    SongInfo(Utilities& utilities);
    QWidget* getWidget();
    void update();
    //sets the text to be displayed
    void setText(QString album, QString artist, QString title, QString track);

private:
    Utilities& util;
    QWidget* widget;
    QGridLayout *temp;
    QLabel *titlelabel;
    QLabel *artistlabel;
    QLabel *albumlabel;
    QFrame *line;

public slots:
    void updateLabels(QString album, QString artist, QString title, QString track);
};

#endif // SONGINFO_H
