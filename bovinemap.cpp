#include "bovinemap.h"
#include <QDebug>
#include <QComboBox>
#include <QDial>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QVariant>

// TODO: kann weg?
BovineMapEntry::BovineMapEntry() {
    this->widget = nullptr;
    this->node = nullptr;
}

BovineMapEntry::BovineMapEntry(QString path, QString propName, QWidget *pWidget,
                               WidgetType wtype) {
    this->node = nullptr;
    this->path = path;
    this->propName = propName;
    this->widget = pWidget;
    this->wtype = wtype;
}

BovineMapEntry::BovineMapEntry(BovineNode *node, QString propName) {
    this->node = node;
    this->path = node->getFullPath();
    this->widget = nullptr;
    this->propName = propName;
}

BovineNode *BovineMapEntry::getNode() const
{
    return node;
}

void BovineMapEntry::setNode(BovineNode *value)
{
    node = value;
}

QWidget *BovineMapEntry::getWidget() const
{
    return widget;
}

QString BovineMapEntry::getPath() const
{
    return path;
}

void BovineMapEntry::setWidget(QWidget *value)
{
    widget = value;
}

WidgetType BovineMapEntry::getWtype() const
{
    return wtype;
}

void BovineMapEntry::setWtype(const WidgetType &value)
{
    wtype = value;
}

void BovineMapEntry::value2ui() {
    if (node == nullptr)
        return;
    QVariant *val = node->getProperty(propName);
    if (val == nullptr)
        return;

    switch(wtype) {
        case COMBO: {
            QString v = val->toString();
            QComboBox *cb = (QComboBox*)widget;
            cb->blockSignals(true);
            cb->setCurrentIndex(cb->findData(v));
            cb->blockSignals(false);
            break;
        }
        case DIAL: {
            QDial *ps = (QDial*)widget;
            ps->blockSignals(true);
            double v = val->toDouble();
            ps->setValue((int)(v*100.));
            ps->blockSignals(false);
            break;
        }
        case SLIDER: {
            QSlider *ps = (QSlider*)widget;
            ps->blockSignals(true);
            double v = val->toDouble();
            ps->setValue((int)(v*100.));
            ps->blockSignals(false);
            break;
        }
        case PUSH_BUTTON: {
            QPushButton *pb = (QPushButton*)widget;
            pb->blockSignals(true);
            int v = val->toInt();
            pb->setChecked(v == 1);
            pb->blockSignals(false);
            break;
        }
        case LINE_EDIT: {
            QLineEdit *pb = (QLineEdit*)widget;
            pb->blockSignals(true);
            pb->setText(val->toString());
            pb->blockSignals(false);
            break;
        }
    }

}
void BovineMapEntry::updateValue(QVariant val, bool updateWidget)
{
    // Update value in node tree
    if (node != nullptr) node->setProperty(propName, val);
    if (!updateWidget || widget == nullptr) return;

    switch(wtype) {
        case COMBO: {
            QString v = val.toString();
            QComboBox *cb = (QComboBox*)widget;
            cb->blockSignals(true);
            cb->setCurrentIndex(cb->findData(v));
            cb->blockSignals(false);
            break;
        }
        case DIAL: {
            QDial *ps = (QDial*)widget;
            ps->blockSignals(true);
            double v = val.toDouble();
            ps->setValue((int)(v*100.));
            ps->blockSignals(false);
            break;
        }
        case SLIDER: {
            QSlider *ps = (QSlider*)widget;
            ps->blockSignals(true);
            double v = val.toDouble();
            ps->setValue((int)(v*100.));
            ps->blockSignals(false);
            break;
        }
        case PUSH_BUTTON: {
            QPushButton *pb = (QPushButton*)widget;
            pb->blockSignals(true);
            int v = val.toInt();
            pb->setChecked(v == 1);
            pb->blockSignals(false);
            break;
        }
        case LINE_EDIT: {
            QLineEdit *pb = (QLineEdit*)widget;
            pb->blockSignals(true);
            pb->setText(val.toString());
            pb->blockSignals(false);
            break;
        }
    }
}

QString BovineMapEntry::getPropName() const
{
    return propName;
}

void BovineMapEntry::setPropName(const QString &value)
{
    propName = value;
}

void BovineMapEntry::setPath(const QString &value)
{
    path = value;
}

/*******************************************************************************************
 * Bovine Map
 */

BovineMap::BovineMap()
{
}

/**
 * @brief BovineMap::add
 * @param node node to be added
 */
void BovineMap::add(BovineNode *node)
{
    QString fpath = node->getFullPath();
    pathMap[fpath] = node;
}


/**
 * @brief BovineMap::add
 * @param node Actual node of the tree containing the properties
 * @param key the key of the property
 * @param value the current value of the property
 *
 * This actually adds properties to the pathmap,
 * even though they aren't really branches of a tree. What an
 * inconsistency. We even need to append "/value". *sighs*
 */
void BovineMap::add(BovineNode *node, QString key)
{
    QString fpath = node->getFullPath();
    pathMap[fpath] = node;

    QString fproppath = fpath + "/" + key + "/value";
    if (prop2pathMap.contains(fproppath)) {
        prop2pathMap[fproppath].setNode(node);
        prop2pathMap[fproppath].setPropName(key);
    }
    else {
        prop2pathMap.insert(fproppath, BovineMapEntry(node, key));
    }
}

void BovineMap::add(QString path, QString propertyName, QWidget *pwidget,
                WidgetType type)
{
    QString fproppath = path + "/" + propertyName + "/value";
    if (prop2pathMap.contains(fproppath)) {
        BovineMapEntry *pe = &prop2pathMap[fproppath];
        pe->setPath(path);
        pe->setPropName(propertyName);
        pe->setWidget(pwidget);
        pe->setWtype(type);
    }
    else {
        BovineMapEntry me = BovineMapEntry(path, propertyName, pwidget, type);
        prop2pathMap.insert(fproppath, me);
    }
    widget2pathMap[pwidget] = &prop2pathMap[fproppath];
}

BovineNode* BovineMap::findByPropValue(QString &propval)
{
    foreach(QString key, pathMap.keys()) {
        BovineNode* n = pathMap[key];
        QHash<QString, QVariant> p = n->getProperties();
        foreach(QString pkey, p.keys()) {
            if (p[pkey].toString() == propval)
                return n;
        }
    }
    return nullptr;
}

BovineMapEntry *BovineMap::find(QString propPath)
{
    if (!prop2pathMap.contains(propPath))
        return nullptr;
    return &prop2pathMap[propPath];
}

BovineMapEntry *BovineMap::find(QWidget *widget)
{
    if (!widget2pathMap.contains(widget))
        return nullptr;
    return widget2pathMap[widget];
}

void BovineMap::printEntries()
{
    qDebug() << "pathmap contains " << pathMap.size() << " entries.";
    qDebug() << "value2pathmap contains " << prop2pathMap.size() << " entries.";
    qDebug() << "widget2pathmap contains " << widget2pathMap.size() << " entries.";
}

void BovineMap::updateUI()
{
    foreach(QWidget *w, widget2pathMap.keys()) {
        widget2pathMap[w]->value2ui();
    }
}

