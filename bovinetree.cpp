#include "bovinetree.h"

BovineTree::BovineTree() :
    rootNode(nullptr)
{

}

BovineTree::~BovineTree() {
    if (rootNode != nullptr) delete rootNode;
    foreach(QString key, prop2pathMap.keys()) {
        delete prop2pathMap[key];
    }
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
        prop2pathMap[fproppath]->setNode(node);
        prop2pathMap[fproppath]->setPropName(key);
    }
    else {
        prop2pathMap.insert(fproppath, new BovineNodeMapping(node, key));
    }
}


/**
 * @brief BovineTree::find
 * @param propPath path to the property
 * @return the first node mapping entry
 */
BovineNodeMapping *BovineTree::find(const QString &propPath)
{
    if (!prop2pathMap.contains(propPath))
        return nullptr;
    return prop2pathMap[propPath];
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
        pe = prop2pathMap[fproppath];
        pe->setPath(path);
        pe->setPropName(propertyName);
        pe->setUserdata(userdata);
    }
    else {
        pe = new BovineNodeMapping(path, propertyName, userdata);
        prop2pathMap.insert(fproppath, pe);
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
