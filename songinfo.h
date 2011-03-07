#ifndef SONGINFO_H
#define SONGINFO_H
#include <QtGui>
#include <QObject>
#include "utilities.h"

class SongInfo: public QObject
{
    Q_OBJECT;
public:
    SongInfo(Utilities& utilities);
    ~SongInfo();
    QWidget* getWidget();
    QWidget* settext(QString album, QString artist, QString song);
    void update();

private:
    Utilities& util;
    QWidget* widget;
    QFontMetrics* fontmetrics;
    QGridLayout *temp;

public slots:
    void updatelabels(QString album, QString artist, QString song);

};

#endif // SONGINFO_H
