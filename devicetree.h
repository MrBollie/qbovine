#ifndef BOVINEMAP_H
#define BOVINEMAP_H

#include "bovinetree.h"
#include "widgetcontainer.h"

class DeviceTree : public BovineTree {
public:
    DeviceTree();

    void add(BovineNode *node, QString key);
    void add(QString path, QString prop, QWidget *pwidget, WidgetType type);
    BovineNode* findByPropValue(QString &propval);
    BovineNodeMapping* find(QWidget *widget);
    BovineNodeMapping* find(const QString &s) {
        return BovineTree::find(s);
    }
    void printEntries();

    QHash<QWidget *, BovineNodeMapping *> getWidget2pathMap() const;

    ~DeviceTree();
private:
    QHash<QWidget*, BovineNodeMapping*> widget2pathMap;
};

#endif // BOVINEMAP_H
