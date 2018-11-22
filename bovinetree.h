#ifndef BOVINETREE_H
#define BOVINETREE_H

#include <QHash>
#include <QString>
#include "bovinenode.h"
#include "bovinenodemapping.h"

class BovineTree
{
public:
    BovineTree();
    void add(BovineNode *node);
    void add(BovineNode *node, QString prop);

    void readInitialTree(const QJsonObject &obj);
private:
    QHash<QString, BovineNodeMapping> prop2pathMap;

protected:
    BovineNode* rootNode;
    QHash<QString, BovineNode*> pathMap;
};

#endif // BOVINETREE_H
