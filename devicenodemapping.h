#ifndef BOVINEMAPENTRY_H
#define BOVINEMAPENTRY_H

#include <QString>
#include <QWidget>
#include "bovinenodemapping.h"

typedef enum __WidgetType {
    COMBO,
    DIAL,
    RADIO,
    PUSH_BUTTON,
    SLIDER,
    LINE_EDIT
} WidgetType;


class DeviceNodeMapping : public BovineNodeMapping
{
public:
    DeviceNodeMapping();

    DeviceNodeMapping(BovineNode *node, const QString &propname);
    DeviceNodeMapping(QString path, QString propName, QWidget* pWidget,
                   WidgetType wtype);

    QWidget *getWidget() const;

    void setWidget(QWidget *value);

    WidgetType getWtype() const;
    void setWtype(const WidgetType &value);

private:
    QWidget *widget;
    WidgetType wtype;
};


#endif // BOVINEMAPENTRY_H
