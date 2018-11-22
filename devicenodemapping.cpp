#include "devicenodemapping.h"

DeviceNodeMapping::DeviceNodeMapping() : BovineNodeMapping ()
{

}

DeviceNodeMapping::DeviceNodeMapping(BovineNode* node,
                                     const QString &propname) :
    BovineNodeMapping (node, propname),
    widget(nullptr)
{

}

DeviceNodeMapping::DeviceNodeMapping(QString path, QString propName, QWidget *pWidget,
                               WidgetType wtype) :
    BovineNodeMapping(path, propName)
{
    this->widget = pWidget;
    this->wtype = wtype;
}


QWidget *DeviceNodeMapping::getWidget() const
{
    return widget;
}


void DeviceNodeMapping::setWidget(QWidget *value)
{
    widget = value;
}

WidgetType DeviceNodeMapping::getWtype() const
{
    return wtype;
}

void DeviceNodeMapping::setWtype(const WidgetType &value)
{
    wtype = value;
}
