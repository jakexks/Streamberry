#ifndef SBSEARCHBAR_H
#define SBSEARCHBAR_H

#include <QFrame>
#include <QLineEdit>
#include <QPushButton>

class SBSearchBar : public QFrame
{
    Q_OBJECT
public:
    explicit SBSearchBar(QString path, QFrame *parent = 0);
signals:
    void newSearchString(QString str);
public slots:
    void checkText(QString text);
    void crossPressed();
    void crossReleased();
    void crossClicked();
private:
    QLineEdit* textbox;
    QPushButton *cross;
    QString expath;
};

#endif // SBSEARCHBAR_H
