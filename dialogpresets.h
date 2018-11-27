#ifndef DIALOGPRESETS_H
#define DIALOGPRESETS_H


#include <QDialog>
#include <QListWidgetItem>
#include "bovineapi.h"

namespace Ui {
class DialogPresets;
}

class DialogPresets : public QDialog
{
    Q_OBJECT

public:
    //explicit DialogPresets(QWidget *parent = nullptr);
    DialogPresets(QWidget *parent, BovineAPI *api);
    DialogPresets(QWidget *parent, BovineAPI *api, const int &fxid);
    ~DialogPresets();

private slots:
    void on_lwFolder_itemClicked(QListWidgetItem *item);
    void on_lwPreset_itemClicked(QListWidgetItem *item);

    void on_pbClose_clicked();

    void on_pbFolderNew_clicked();

    void on_pbFolderRename_clicked();

    void on_pbFolderDelete_clicked();

    void on_pbPresetSaveAs_clicked();

    void on_pbPresetSave_clicked();

    void on_pbPresetRename_clicked();

    void on_pbPresetDelete_clicked();

    void on_pbPresetFavorite_clicked();

    void on_pbLoad_clicked();

private:
    Ui::DialogPresets *ui;
    BovineAPI *api;
    BovineNode *selectedPreset;
};

#endif // DIALOGPRESETS_H
