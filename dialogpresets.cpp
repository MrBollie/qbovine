#include "dialogpresets.h"
#include "ui_dialogpresets.h"
#include <QDebug>
#include <QIcon>

DialogPresets::DialogPresets(QWidget *parent, BovineAPI *api) :
    QDialog(parent),
    ui(new Ui::DialogPresets),
    selectedPreset(nullptr)
{
    ui->setupUi(this);
    this->api = api;

    BovineNode *pre_folders = api->getDevicePresetFolders();

    if (pre_folders != nullptr) {
        foreach(BovineNode* n, *pre_folders->getChildren()) {
            QListWidgetItem *item = new QListWidgetItem(
                        n->getProperty("name")->toString());
            item->setData(Qt::UserRole, n->getPath());
            ui->lwFolder->addItem(item);
        }
    }

    ui->pbFolderRename->setEnabled(false);
    ui->pbFolderDelete->setEnabled(false);

    ui->pbPresetDelete->setEnabled(false);
    ui->pbPresetFavorite->setEnabled(false);
    ui->pbPresetRename->setEnabled(false);
    ui->pbPresetSave->setEnabled(false);
    ui->pbPresetSaveAs->setEnabled(false);

}

/**
 * @brief DialogPresets::DialogPresets
 * @param parent Parent window
 * @param api pointer to the API layer
 * @param fxid ID of the FX to show presets for
 * @details This constructor can be used for FX presets.
 */
DialogPresets::DialogPresets(QWidget *parent, BovineAPI *api, const int &fxid) :
    QDialog(parent),
    ui(new Ui::DialogPresets)
{
    ui->setupUi(this);
    this->api = api;
}

/**
 * @brief DialogPresets::~DialogPresets
 */
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
    selectedPreset = nullptr;
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
    QString uid = item->data(Qt::UserRole).toString();
    BovineNode *n = api->getDevicePreset(uid);
    selectedPreset = n;
    if (n) {
        ui->pbPresetFavorite->setEnabled(true);
        QVariant* fav = n->getProperty("favorite");
        QVariant* factory = n->getProperty("factory");

        if (!factory || !factory->toBool()) {
            ui->pbPresetSave->setEnabled(true);
            ui->pbPresetDelete->setEnabled(true);
        }
        else {
            ui->pbPresetRename->setEnabled(true);
            ui->pbPresetSave->setEnabled(false);
            ui->pbPresetDelete->setEnabled(false);
        }

        if (fav && fav->toBool()) {
            ui->pbPresetFavorite->setEnabled(true);
        }
        ui->pbPresetSaveAs->setEnabled(true);
    }
    else {
        // Enable all preset buttons
        ui->pbPresetDelete->setEnabled(false);
        ui->pbPresetSave->setEnabled(false);
        ui->pbPresetFavorite->setEnabled(false);
        ui->pbPresetSaveAs->setEnabled(true);
    }
}

void DialogPresets::on_pbClose_clicked()
{
    close();
}

void DialogPresets::on_pbFolderNew_clicked()
{

}

void DialogPresets::on_pbFolderRename_clicked()
{

}

void DialogPresets::on_pbFolderDelete_clicked()
{

}

void DialogPresets::on_pbPresetSaveAs_clicked()
{

}

void DialogPresets::on_pbPresetSave_clicked()
{

}

void DialogPresets::on_pbPresetRename_clicked()
{

}

void DialogPresets::on_pbPresetDelete_clicked()
{

}

void DialogPresets::on_pbPresetFavorite_clicked()
{

}

void DialogPresets::on_pbLoad_clicked()
{
    if (selectedPreset) {
        QVariant *uid = selectedPreset->getProperty("uid");
        if (uid)
            api->loadDevicePreset(uid->toString());
    }
}
