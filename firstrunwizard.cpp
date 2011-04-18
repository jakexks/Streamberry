#include "firstrunwizard.h"
#include <QtGui>
#include <QDebug>
#include "database.h"
#include "filescan.h"

FirstRunWizard::FirstRunWizard(Database &datab, Filescan &fscan, QWidget *parent) : QWizard(parent), db(datab), fs(fscan)
{
    addPage(new NickPage);
    sharingpage = new SharingPage;
    addPage(sharingpage);
    QObject::connect(this, SIGNAL(accepted()), &fs, SLOT(scan()));
    setWindowTitle(tr("Streamberry first run wizard"));
}

//Sets the nickname and selected folders in the database and runs a filescan
void FirstRunWizard::accept()
{
    db.setNick(field("nickname").toString());
    db.setFolders(sharingpage->getSelectedFiles());
    QDialog::accept();
}


NickPage::NickPage(QWidget *parent) : QWizardPage(parent)
{
    QLabel *intro = new QLabel(tr("Welcome to the first run wizard for Streamberry. Please select a nickname so that other people on your network can identify you."));
    intro->setWordWrap(true);
    QLineEdit *nickedit = new QLineEdit();
    nickedit->setMaxLength(20);
    QVBoxLayout *layout = new QVBoxLayout;
    registerField("nickname*",nickedit);
    layout->addWidget(intro);
    layout->addSpacing(10);
    layout->addWidget(nickedit);
    setLayout(layout);
}


SharingPage::SharingPage(QWidget *parent) : QWizardPage(parent)
{
    QLabel *label = new QLabel(tr("Please select which files and folders you would like to appear in your media library and share with others on your local network."));
    label->setWordWrap(true);

    tree = new QTreeView(this);
    model = new QFileSystemModel;
    model->setRootPath("");
    tree->setModel(model);
    tree->setSelectionMode(QAbstractItemView::MultiSelection);
    registerField("tree",tree);

    QVBoxLayout *layout = new QVBoxLayout;

    expandRows(QDir::homePath() + "/Music");

    tree->setColumnWidth(0,300);
    layout->addWidget(label);
    layout->addWidget(tree);
    setLayout(layout);
}

// Expands all rows needed to show the current selection
void SharingPage::expandRows(QString filepath)
{
    if(model->index(filepath).parent() == QModelIndex())
    {
        tree->setExpanded(model->index(filepath),true);
        return;
    }
    expandRows(model->filePath(model->index(filepath).parent()));
    tree->setExpanded(model->index(filepath),true);
}

QString SharingPage::getSelectedFiles()
{
    QModelIndexList sindexes = tree->selectionModel()->selectedRows();
    int sindlength = sindexes.length();
    QStringList strs;
    for(int i = 0; i < sindlength; i++)
    {
        strs.append(model->filePath(sindexes.at(i)));
    }
    return strs.join(";");
}
