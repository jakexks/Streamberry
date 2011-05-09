#ifndef SBSEARCHBAR_H
#define SBSEARCHBAR_H

#include <QFrame>
#include <QLineEdit>
#include <QPushButton>

//search bar in top bar
class SBSearchBar : public QFrame
{
    Q_OBJECT
public:
    explicit SBSearchBar(QString path, QFrame *parent = 0);
    //set text in text box
    void setText(QString text);
signals:
    //search string has changed
    void newSearchString(QString str);
public slots:
    //check whether to show cross
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
