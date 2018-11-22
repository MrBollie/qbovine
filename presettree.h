#ifndef PRESETTREE_H
#define PRESETTREE_H

#include "bovinetree.h"


class PresetTree : public BovineTree
{
public:
    PresetTree();
    void readInitialTree(const QJsonObject &obj);
    BovineNodeMapping *findByUID(const QString &uid);
private:
    QHash<QString, BovineNodeMapping*> uidMap;
};

#endif // PRESETTREE_H
