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
    tabwidget->addTab(new MiscTab(), "&Misc");
    tabwidget->addTab(stab, "&Sharing");
    layout->addWidget(tabwidget);

    QDialogButtonBox *buttonbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonbox, SIGNAL(accepted()), this, SLOT(ok()));
    connect(buttonbox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(this, SIGNAL(accept()), this, SLOT(close()));
    layout->addWidget(buttonbox);

    this->setWindowTitle("Settings");
    this->setLayout(layout);
}

void SettingsDialog::ok()
{
    QString strlist = stab->getSelectedFilepaths();
    db.setFolders(strlist);
    emit(accept());
}
