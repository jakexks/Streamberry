#include "sbsearchbar.h"
#include <QPainter>
#include <QGridLayout>
#include <QtGui>
#include <QMacStyle>
#include <QDebug>
#include <QString>

SBSearchBar::SBSearchBar(QString path, QFrame *parent) :
        QFrame(parent)
{
    expath = path;
    QGridLayout *layout = new QGridLayout(this);
    QImage icon(path + "images/searchBarMagnifying.png");
    QPushButton* icondisplay = new QPushButton();
    icondisplay->setStyleSheet("border:none;");
    icondisplay->setFlat(true);
    icondisplay->setFixedSize(icon.size());
    cross = new QPushButton();
    textbox = new QLineEdit();

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);
    setStyleSheet("QFrame {background-color: white; color:#969696}");
    setMinimumSize(170, 21);
    setMaximumSize(170, 21);

    layout->setSpacing(3);
    layout->setMargin(2);
    layout->setColumnStretch(0,0);
    layout->setColumnStretch(1,1);
    layout->setColumnStretch(2,0);

    icondisplay->setIcon(QIcon(QPixmap::fromImage(icon)));
    //icondisplay->setPixmap(QPixmap::fromImage(icon));

    QFont textfont = textbox->font();
    textfont.setPixelSize(12);

    textbox->setPlaceholderText("Search...");
    textbox->setModified(true);
    textbox->setFrame(false);
    textbox->setTextMargins(0,0,0,0);
    textbox->setFont(textfont);
    textbox->setFixedHeight(17);
    textbox->setStyleSheet("margin:0px; padding:0px;");
    #ifdef Q_OS_MAC
      QMacStyle::setFocusRectPolicy(textbox, QMacStyle::FocusDisabled);
    #endif


    QString temp = "background-image:url(";
    temp += path;
    temp += "images/searchBarCross.png); background-repeat:repeat-none; border:none;";
    cross->setStyleSheet(temp);
    cross->setMaximumSize(16, 14);
    cross->setFlat(true);
    cross->setVisible(false);

    layout->addWidget(icondisplay, 0,0);
    layout->addWidget(textbox, 0, 1);
    layout->addWidget(cross, 0, 2);

    connect(textbox, SIGNAL(textChanged(QString)), this, SLOT(checkText(QString)));
    connect(cross, SIGNAL(pressed(void)), this, SLOT(crossPressed(void)));
    connect(cross, SIGNAL(released(void)), this, SLOT(crossReleased(void)));
    connect(cross, SIGNAL(clicked(void)), this, SLOT(crossClicked(void)));
}

void SBSearchBar::checkText(QString text) {
    emit newSearchString(text);

    if(text.length()>0) {
        cross->setVisible(true);
        return;
    }

    cross->setVisible(false);
}

void SBSearchBar::crossPressed()
{
    QString temp = "background-image:url(";
    temp += expath;
    temp += "images/searchBarCrossDown.png); background-repeat:repeat-none; border:none;";
    cross->setStyleSheet(temp);
}

void SBSearchBar::crossReleased()
{
    QString temp = "background-image:url(";
    temp += expath;
    temp += "images/searchBarCross.png); background-repeat:repeat-none; border:none;";
    cross->setStyleSheet(temp);
}

void SBSearchBar::crossClicked()
{
    textbox->setText("");
    emit newSearchString("");
}
