#include "bovinetree.h"

BovineTree::BovineTree() :
    rootNode(nullptr)
{

}


/**
 * @brief BivuneTree::add
 * @param node Actual node of the tree containing the properties
 * @param key the key of the property
 * @param value the current value of the property
 */
void BovineTree::add(BovineNode *node, QString key)
{
    QString fpath = node->getFullPath();
    pathMap[fpath] = node;

    QString fproppath = fpath + "/" + key + "/value";
    if (prop2pathMap.contains(fproppath)) {
        prop2pathMap[fproppath].setNode(node);
        prop2pathMap[fproppath].setPropName(key);
    }
    else {
        prop2pathMap.insert(fproppath, BovineNodeMapping(node, key));
    }
}

/**
 * @brief BovineTree::add
 * @param path
 * @param propertyName
 * @param userdata
 */
BovineNodeMapping* BovineTree::add(const QString &path,
                                   const QString &propertyName,
                                   void *userdata)
{
    BovineNodeMapping *pe = nullptr;
    QString fproppath = path + "/" + propertyName + "/value";
    if (prop2pathMap.contains(fproppath)) {
        pe = &prop2pathMap[fproppath];
        pe->setPath(path);
        pe->setPropName(propertyName);
        pe->setUserdata(userdata);
    }
    else {
        BovineNodeMapping me = BovineNodeMapping(path, propertyName, userdata);
        prop2pathMap.insert(fproppath, me);
        pe = &me;
    }
    return pe;
}

/**
 * @brief BovineMap::readInitialTree
 * @param obj
 * @details Builds the node tree
 */
void BovineTree::readInitialTree(const QJsonObject &obj)
{
    if (rootNode != nullptr) {
        delete rootNode;
    }
    rootNode = new BovineNode(obj, this);
}
