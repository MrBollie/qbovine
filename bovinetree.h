#ifndef BOVINETREE_H
#define BOVINETREE_H

#include <QHash>
#include <QString>
#include <QDebug>
#include "bovinenode.h"
#include "bovinenodemapping.h"

class BovineTree
{
public:
    BovineTree();
    ~BovineTree();
    void add(BovineNode *node);
    void add(BovineNode *node, QString prop);
    BovineNodeMapping* add(const QString &path, const QString &prop,
                           void *userdata);

    void readInitialTree(const QJsonObject &obj);
    void foo() { qDebug() << "FOOOOOOOOOOOOOOOOOOOOOOOOOO";}

    BovineNodeMapping *find(const QString &propPath);
protected:
    QHash<QString, BovineNodeMapping*> prop2pathMap;
    BovineNode* rootNode;
    QHash<QString, BovineNode*> pathMap;
};

#endif // BOVINETREE_H
