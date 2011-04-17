#include "settingsdialog.h"
#include <QtGui>
#include "misctab.h"
#include "sharingtab.h"
#include "database.h"

#include <QDebug>

SettingsDialog::SettingsDialog(Database &datab, QWidget *parent) : db(datab)
{
    QVBoxLayout *layout = new QVBoxLayout;

    QTabWidget *tabwidget = new QTabWidget();
    stab = new SharingTab(db.getFolders(0));
    tabwidget->addTab(stab, "&Sharing");
    tabwidget->addTab(new MiscTab(), "&Misc");
    layout->addWidget(tabwidget);

    QDialogButtonBox *buttonbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonbox, SIGNAL(accepted()), this, SLOT(ok()));
    connect(buttonbox, SIGNAL(rejected()), this, SLOT(reject()));
    layout->addWidget(buttonbox);

    this->setWindowTitle("Settings");
    this->setLayout(layout);
    this->setFixedSize(500,550);
}

void SettingsDialog::ok()
{
    QString strlist = stab->getSelectedFilepaths();
    db.setFolders(strlist);
    QDialog::accept();
}
