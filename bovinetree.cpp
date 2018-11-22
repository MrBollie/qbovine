#include "bovinetree.h"

BovineTree::BovineTree()
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
