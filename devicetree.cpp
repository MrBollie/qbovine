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


void DeviceTree::add(QString path, QString propertyName, QWidget *pwidget,
                WidgetType type)
{
    BovineNodeMapping *pme = BovineTree::add(path, propertyName, (void*)nullptr);
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

DeviceNodeMapping *DeviceTree::find(QString propPath)
{
    if (!prop2pathMap.contains(propPath))
        return nullptr;
    return &prop2pathMap[propPath];
}

DeviceNodeMapping *DeviceTree::find(QWidget *widget)
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


QHash<QWidget *, DeviceNodeMapping *> DeviceTree::getWidget2pathMap() const
{
    return widget2pathMap;
}

