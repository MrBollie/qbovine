#ifndef BOVINEMAP_H
#define BOVINEMAP_H

#include "bovinenode.h"

typedef enum __WidgetType {
    COMBO,
    DIAL,
    RADIO,
    PUSH_BUTTON,
    SLIDER,
    LINE_EDIT
} WidgetType;


class BovineMapEntry {
public:
    BovineMapEntry();
    BovineMapEntry(QString path, QString propName, QWidget* pWidget,
                   WidgetType wtype);

    BovineMapEntry(BovineNode *node, QString propName);
    BovineNode *getNode() const;

    void setNode(BovineNode *value);

    QWidget *getWidget() const;

    QString getPath() const;

    void setWidget(QWidget *value);

    WidgetType getWtype() const;
    void setWtype(const WidgetType &value);

    void updateValue(QVariant val, bool updateWidget = false);

    QString getPropName() const;
    void setPropName(const QString &value);

    void setPath(const QString &value);

    void value2ui();
private:
    BovineNode *node;
    QWidget *widget;
    WidgetType wtype;
    QString path;
    QString propName;
};


class BovineMap {
public:
    BovineMap();
    void add(BovineNode *node);
    void add(BovineNode *node, QString prop);
    void add(QString path, QString prop, QWidget *pwidget, WidgetType type);
    BovineMapEntry* find(QString propPath);
    BovineMapEntry* find(QWidget *widget);
    void printEntries();
    void updateUI();

private:
    QHash<QString, BovineNode*> pathMap;
    QHash<QWidget*, BovineMapEntry*> widget2pathMap;
    QHash<QString, BovineMapEntry> prop2pathMap;
};

#endif // BOVINEMAP_H
