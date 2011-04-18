#include "settingsdialog.h"
#include <QtGui>
#include "misctab.h"
#include "sharingtab.h"
#include "database.h"
#include "filescan.h"

#include <QDebug>

SettingsDialog::SettingsDialog(Database &datab, Filescan &fscan, QWidget *parent) : db(datab), fs(fscan)
{
    QVBoxLayout *layout = new QVBoxLayout;

    QTabWidget *tabwidget = new QTabWidget();
    stab = new SharingTab(db.getFolders(0));
    mtab = new MiscTab(db.getNick(), fs);
    tabwidget->addTab(stab, "&Sharing");
    tabwidget->addTab(mtab, "&Misc");
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
    db.setNick(mtab->getNick());
    db.setFolders(stab->getSelectedFilepaths());
    QDialog::accept();
}
