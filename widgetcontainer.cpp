#include "widgetcontainer.h"

WidgetContainer::WidgetContainer()
{

}

WidgetContainer::WidgetContainer(QWidget *widget, const WidgetType &type)
{
    this->widget = widget;
    this->type = type;
}

QWidget *WidgetContainer::getWidget() const
{
    return widget;
}

void WidgetContainer::setWidget(QWidget *value)
{
    widget = value;
}

WidgetType WidgetContainer::getType() const
{
    return type;
}

void WidgetContainer::setType(const WidgetType &value)
{
    type = value;
}
