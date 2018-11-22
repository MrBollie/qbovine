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

/**
 * @brief DeviceTree::add
 * @param node Actual node of the tree containing the properties
 * @param key the key of the property
 * @param value the current value of the property
 *
 * This actually adds properties to the pathmap,
 * even though they aren't really branches of a tree. What an
 * inconsistency. We even need to append "/value". *sighs*
 */
void DeviceTree::add(BovineNode *node, QString key)
{
    QString fpath = node->getFullPath();
    pathMap[fpath] = node;

    QString fproppath = fpath + "/" + key + "/value";
    if (prop2pathMap.contains(fproppath)) {
        prop2pathMap[fproppath].setNode(node);
        prop2pathMap[fproppath].setPropName(key);
    }
    else {
        prop2pathMap.insert(fproppath, DeviceNodeMapping(node, key));
    }
}

void DeviceTree::add(QString path, QString propertyName, QWidget *pwidget,
                WidgetType type)
{
    QString fproppath = path + "/" + propertyName + "/value";
    if (prop2pathMap.contains(fproppath)) {
        DeviceNodeMapping *pe = &prop2pathMap[fproppath];
        pe->setPath(path);
        pe->setPropName(propertyName);
        pe->setWidget(pwidget);
        pe->setWtype(type);
    }
    else {
        DeviceNodeMapping me = DeviceNodeMapping(path, propertyName, pwidget, type);
        prop2pathMap.insert(fproppath, me);
    }
    widget2pathMap[pwidget] = &prop2pathMap[fproppath];
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

