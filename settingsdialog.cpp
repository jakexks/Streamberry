#include "settingsdialog.h"
#include <QtGui>
#include "database.h"
#include "filescan.h"

#include <QDebug>

SettingsDialog::SettingsDialog(Database &datab, Filescan &fscan, QWidget *parent) : db(datab), fs(fscan)
{
    nickedit = new QLineEdit();
    nickedit->setMaxLength(20);
    nickedit->setText(db.getNick());

    model = new QFileSystemModel;
    model->setRootPath("");
    tree = new QTreeView(this);
    tree->setModel(model);
    tree->setSelectionMode(QAbstractItemView::MultiSelection);
    tree->setColumnWidth(0,300);
    tree->setFixedHeight(350);
    expandRows(QDir::homePath() + "/Music");
    QStringList selectedfiles = db.getFolders(0);
    int sflength = selectedfiles.length();
    for(int i = 0; i < sflength; i++)
    {
        tree->selectionModel()->select(model->index(selectedfiles.at(i)),QItemSelectionModel::SelectCurrent);
        expandRows(model->filePath(model->index(selectedfiles.at(i)).parent()));
    }
    QLabel *label = new QLabel(tr("Please select which files and folders you would like to appear in your media library and share with others on your local network."));
    label->setWordWrap(true);

    QPushButton *scanbutton = new QPushButton("&Scan");
    QObject::connect(scanbutton, SIGNAL(clicked()), this, SLOT(scanStarting()));
    QObject::connect(this, SIGNAL(startScan()), &fs, SLOT(scan()));
    QObject::connect(&fs, SIGNAL(finishedFileScan()), this, SLOT(scanFinished()));
    QProgressBar *pbar = new QProgressBar();
    pbar->setMaximum(0);
    sw = new QStackedWidget();
    sw->addWidget(scanbutton);
    sw->addWidget(pbar);
    if(fs.currentlyScanning() == true)
    {
        sw->setCurrentIndex(1);
    }

    QDialogButtonBox *buttonbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonbox, SIGNAL(accepted()), this, SLOT(ok()));
    connect(buttonbox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *toplayout = new QVBoxLayout;
    QFormLayout *nicklayout = new QFormLayout;
    QFormLayout *scanlayout = new QFormLayout;
    nicklayout->addRow("&Nickname:", nickedit);
    toplayout->addLayout(nicklayout);
    toplayout->addSpacing(10);
    toplayout->addWidget(label);
    toplayout->addWidget(tree);
    toplayout->addSpacing(10);
    scanlayout->addRow("Scan folders for media:", sw);
    toplayout->addLayout(scanlayout);
    toplayout->addWidget(buttonbox);

    #ifdef Q_WS_WIN
        this->setWindowTitle("Settings");
    #endif

    #ifdef Q_WS_MAC
        this->setWindowTitle("Preferences");
    #endif

    #ifdef Q_WS_X11
        this->setWindowTitle("Preferences");
    #endif

    this->setLayout(toplayout);
    this->setFixedSize(500,550);
}

SettingsDialog::~SettingsDialog()
{
    delete model;
}

void SettingsDialog::ok()
{
    db.setNick(nickedit->text());
    db.setFolders(getSelectedFilepaths());
    QDialog::accept();
}

// Expands all rows needed to show the current selection
void SettingsDialog::expandRows(QString filepath)
{
    if(model->index(filepath).parent() == QModelIndex())
    {
        tree->setExpanded(model->index(filepath),true);
        return;
    }
    expandRows(model->filePath(model->index(filepath).parent()));
    tree->setExpanded(model->index(filepath),true);
}

// Returns a semicolon separated string of the filepaths the user selected
QString SettingsDialog::getSelectedFilepaths()
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

void SettingsDialog::scanStarting()
{
    sw->setCurrentIndex(1);
    db.setFolders(getSelectedFilepaths());
    emit startScan();
}

void SettingsDialog::scanFinished()
{
    sw->setCurrentIndex(0);
}
