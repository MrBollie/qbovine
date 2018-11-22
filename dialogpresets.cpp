#include "dialogpresets.h"
#include "ui_dialogpresets.h"
#include <QDebug>
#include <QIcon>

DialogPresets::DialogPresets(QWidget *parent, BovineAPI *api) :
    QDialog(parent),
    ui(new Ui::DialogPresets)
{
    ui->setupUi(this);
    this->api = api;

    BovineNode *pre_folders = api->getDevicePresetFolders();

    if (pre_folders != nullptr) {
        foreach(BovineNode* n, *pre_folders->getChildren()) {
            QListWidgetItem *item = new QListWidgetItem(n->getProperty("name")->toString());
            item->setData(Qt::UserRole, n->getPath());
            ui->lwFolder->addItem(item);
        }
    }

    ui->pbFolderRename->setEnabled(false);
    ui->pbFolderDelete->setEnabled(false);

    ui->pbPresetDelete->setEnabled(false);
    ui->pbPresetFavorite->setEnabled(false);
    ui->pbPresetSave->setEnabled(false);
    ui->pbPresetSaveAs->setEnabled(false);

}


DialogPresets::~DialogPresets()
{
    delete ui;
}


/**
 * @brief DialogPresets::on_lwFolder_itemClicked
 * @param item selected folder item
 * @details
 */
void DialogPresets::on_lwFolder_itemClicked(QListWidgetItem *item)
{
    ui->lwPreset->reset();
    while (ui->lwPreset->count() > 0) {
        QListWidgetItem* i = ui->lwPreset->takeItem(0);
        delete i;
    }
    QString uid = item->data(Qt::UserRole).toString();
    qDebug() << "Selected UID:" << uid;
    BovineNode *node = api->getDevicePresets(uid);

    // Factory folders cannot be changed or deleted
    QVariant* factory = node->getProperty("factory");
    if (factory != nullptr && factory->toBool()) {
        ui->pbFolderDelete->setEnabled(false);
        ui->pbFolderRename->setEnabled(false);
    }
    else {
        ui->pbFolderDelete->setEnabled(true);
        ui->pbFolderRename->setEnabled(true);
    }

    if (node != nullptr) {
        foreach(BovineNode* n, *node->getChildren()) {
            QListWidgetItem *pres_item = new QListWidgetItem(
                        n->getProperty("name")->toString()
            );
            pres_item->setData(Qt::UserRole, n->getPath());
            ui->lwPreset->addItem(pres_item);
        }
    }

    // Disable all preset buttons, except new
    ui->pbPresetDelete->setEnabled(false);
    ui->pbPresetFavorite->setEnabled(false);
    ui->pbPresetSave->setEnabled(false);
    ui->pbPresetSaveAs->setEnabled(true);
}

void DialogPresets::on_lwPreset_itemClicked(QListWidgetItem *item)
{
    api->loadDevicePreset(item->data(Qt::UserRole).toString());
    // Enable all preset buttons
    ui->pbPresetDelete->setEnabled(true);
    ui->pbPresetFavorite->setEnabled(true);
    ui->pbPresetSave->setEnabled(true);
    ui->pbPresetSaveAs->setEnabled(true);
}

void DialogPresets::on_pbClose_clicked()
{
    close();
}
