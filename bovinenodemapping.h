#ifndef TREEENTRY_H
#define TREEENTRY_H

#include <QString>
#include "bovinenode.h"

class BovineNodeMapping
{
public:
    BovineNodeMapping();
    BovineNodeMapping(const QString &path, const QString &propname,
                      void* userdata = nullptr);
    BovineNodeMapping(BovineNode *node, QString propName);

    BovineNode *getNode() const;
    void setNode(BovineNode *value);

    QString getPath() const;
    void setPath(const QString &value);

    QString getPropName() const;
    void setPropName(const QString &value);

    QVariant *getValue();
    void updateValue(QVariant val);
    void *getUserdata() const;
    void setUserdata(void *value);

private:
    BovineNode *node;
    QString path;
    QString propName;
    void *userdata;
};

#endif // TREEENTRY_H
