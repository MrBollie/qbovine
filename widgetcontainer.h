#ifndef WIDGETCONTAINER_H
#define WIDGETCONTAINER_H

#include <QWidget>


typedef enum __WidgetType {
    COMBO,
    DIAL,
    RADIO,
    PUSH_BUTTON,
    SLIDER,
    LINE_EDIT
} WidgetType;


class WidgetContainer
{
public:
    WidgetContainer();
    WidgetContainer(QWidget* widget, const WidgetType &type);
    QWidget *getWidget() const;
    void setWidget(QWidget *value);

    WidgetType getType() const;
    void setType(const WidgetType &value);

private:
    QWidget *widget;
    WidgetType type;
};

#endif // WIDGETCONTAINER_H
