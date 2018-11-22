#include "devicetree.h"
#include <QDebug>
#include <QComboBox>
#include <QDial>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QVariant>



/*******************************************************************************************
 * Bovine Map
 */

DeviceTree::DeviceTree() :
    BovineTree ()
{
}

DeviceTree::~DeviceTree() {
    foreach(QWidget* w, widget2pathMap.keys()) {
        BovineNodeMapping* nm = widget2pathMap[w];
        if (nm != nullptr) {
            WidgetContainer* wc = (WidgetContainer*)nm->getUserdata();
            if (wc != nullptr)
                delete wc;
        }
    }
}


/**
 * @brief DeviceTree::add
 * @param path
 * @param propertyName
 * @param pwidget
 * @param type
 * @details Adding a widget mapping to the device tree.
 */
void DeviceTree::add(QString path, QString propertyName, QWidget *pwidget,
                WidgetType type)
{
    WidgetContainer* wc = new WidgetContainer(pwidget, type);
    BovineNodeMapping *pme = BovineTree::add(path, propertyName, wc);
    widget2pathMap[pwidget] = pme;
}

BovineNode* DeviceTree::findByPropValue(QString &propval)
{
    foreach(QString key, pathMap.keys()) {
        BovineNode* n = pathMap[key];
        QHash<QString, QVariant>* p = n->getProperties();
        foreach(QString pkey, p->keys()) {
            if ((*p)[pkey].toString() == propval)
                return n;
        }
    }
    return nullptr;
}


BovineNodeMapping *DeviceTree::find(QWidget *widget)
{
    if (!widget2pathMap.contains(widget))
        return nullptr;
    return widget2pathMap[widget];
}

void DeviceTree::printEntries()
{
    qDebug() << "pathmap contains " << pathMap.size() << " entries.";
    qDebug() << "value2pathmap contains " << prop2pathMap.size() << " entries.";
    qDebug() << "widget2pathmap contains " << widget2pathMap.size() << " entries.";
}


QHash<QWidget *, BovineNodeMapping *> DeviceTree::getWidget2pathMap() const
{
    return widget2pathMap;
}

