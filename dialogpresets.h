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
    ~DialogPresets();

private slots:
    void on_lwFolder_itemClicked(QListWidgetItem *item);
    void on_lwPreset_itemClicked(QListWidgetItem *item);

    void on_pbClose_clicked();

private:
    Ui::DialogPresets *ui;
    BovineAPI *api;
};

#endif // DIALOGPRESETS_H
