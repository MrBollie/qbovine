#ifndef BOVINEMAP_H
#define BOVINEMAP_H

#include "bovinetree.h"

typedef enum __WidgetType {
    COMBO,
    DIAL,
    RADIO,
    PUSH_BUTTON,
    SLIDER,
    LINE_EDIT
} WidgetType;

class DeviceTree : public BovineTree {
public:
    DeviceTree();

    void add(BovineNode *node, QString key);
    void add(QString path, QString prop, QWidget *pwidget, WidgetType type);
    BovineNode* findByPropValue(QString &propval);
    BovineNodeMapping* find(QString propPath);
    BovineNodeMapping* find(QWidget *widget);
    void printEntries();

    QHash<QWidget *, BovineNodeMapping *> getWidget2pathMap() const;

private:
    QHash<QWidget*, BovineNodeMapping*> widget2pathMap;
};

#endif // BOVINEMAP_H
