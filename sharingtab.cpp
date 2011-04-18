#include "sharingtab.h"

#include <QDebug>

SharingTab::SharingTab(QStringList sfiles, QWidget *parent) : QWidget(parent), selectedfiles(sfiles)
{
    QVBoxLayout *layout = new QVBoxLayout;

    model = new QFileSystemModel;
    model->setRootPath("");
    tree = new QTreeView(this);
    tree->setModel(model);
    tree->setSelectionMode(QAbstractItemView::MultiSelection);

    expandRows(QDir::homePath() + "/Music");
    int sflength = selectedfiles.length();
    for(int i = 0; i < sflength; i++)
    {
        tree->selectionModel()->select(model->index(selectedfiles.at(i)),QItemSelectionModel::SelectCurrent);
        expandRows(model->filePath(model->index(selectedfiles.at(i)).parent()));
    }
    tree->setColumnWidth(0,300);
    QLabel *label = new QLabel(tr("Please select which files and folders you would like to appear in your media library and share with others on your local network."));
    label->setWordWrap(true);
    layout->addWidget(label);
    layout->addWidget(tree);
    setLayout(layout);
}

// Expands all rows needed to show the current selection
void SharingTab::expandRows(QString filepath)
{
    if(model->index(filepath).parent() == QModelIndex())
    {
        tree->setExpanded(model->index(filepath),true);
        return;
    }
    expandRows(model->filePath(model->index(filepath).parent()));
    tree->setExpanded(model->index(filepath),true);
}

SharingTab::~SharingTab()
{
    delete model;
}

// Returns a semicolon separated string of the filepaths the user selected
QString SharingTab::getSelectedFilepaths()
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
