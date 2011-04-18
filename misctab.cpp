#include "misctab.h"
#include "filescan.h"

#include <QDebug>

MiscTab::MiscTab(QString currentnick, Filescan &fscan, QWidget *parent) : QWidget(parent), cnick(currentnick), fs(fscan)
{
    nickedit = new QLineEdit();
    nickedit->setMaxLength(20);
    nickedit->setText(cnick);

    QPushButton *scanbutton = new QPushButton("S&can");
    QObject::connect(scanbutton, SIGNAL(clicked()), &fs, SLOT(scan()));
    QObject::connect(scanbutton, SIGNAL(clicked()), this, SLOT(scanStarted()));
    QObject::connect(&fs, SIGNAL(finishedFileScan()), this, SLOT(scanFinished()));
    QProgressBar *pbar = new QProgressBar();
    pbar->setMaximum(0);
    sw = new QStackedWidget();
    sw->addWidget(scanbutton);
    sw->addWidget(pbar);

    QFormLayout *layout = new QFormLayout;
    layout->addRow("&Nickname:", nickedit);
    layout->addRow("Scan folders for media", sw);

    setLayout(layout);
}

QString MiscTab::getNick()
{
    return nickedit->text();
}

void MiscTab::scanStarted()
{
    sw->setCurrentIndex(1);
}

void MiscTab::scanFinished()
{
    sw->setCurrentIndex(0);
}
