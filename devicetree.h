#ifndef BOVINEMAP_H
#define BOVINEMAP_H

#include "bovinetree.h"
#include "devicenodemapping.h"

class DeviceTree : public BovineTree {
public:
    DeviceTree();

    void add(BovineNode *node, QString key);
    void add(QString path, QString prop, QWidget *pwidget, WidgetType type);
    BovineNode* findByPropValue(QString &propval);
    DeviceNodeMapping* find(QString propPath);
    DeviceNodeMapping* find(QWidget *widget);
    void printEntries();

    QHash<QWidget *, DeviceNodeMapping *> getWidget2pathMap() const;

private:
    QHash<QWidget*, DeviceNodeMapping*> widget2pathMap;
    QHash<QString, DeviceNodeMapping> prop2pathMap;
};

#endif // BOVINEMAP_H
