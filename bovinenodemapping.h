#ifndef TREEENTRY_H
#define TREEENTRY_H

#include <QString>
#include "bovinenode.h"

class BovineNodeMapping
{
public:
    BovineNodeMapping();
    BovineNodeMapping(const QString &path, const QString &propname);
    BovineNodeMapping(BovineNode *node, QString propName);

    BovineNode *getNode() const;
    void setNode(BovineNode *value);

    QString getPath() const;
    void setPath(const QString &value);

    QString getPropName() const;
    void setPropName(const QString &value);

    QVariant *getValue();
    void updateValue(QVariant val);
private:
    BovineNode *node;
    QString path;
    QString propName;
};

#endif // TREEENTRY_H
