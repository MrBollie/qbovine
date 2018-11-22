#include "presettree.h"

PresetTree::PresetTree()
{

}


/**
 * @brief PresetTree::findByUID
 * @param uid uid of the preset that is to be found
 * @return the mapping entry containing the preset or nullptr
 */
BovineNodeMapping* PresetTree::findByUID(const QString &uid) {
    if (!uidMap.contains(uid)) return nullptr;
    return uidMap[uid];
}


/**
 * @brief PresetTree::readInitialTree
 * @param obj JSON object to be parsed
 * @details parses the JSON object.
 */
void PresetTree::readInitialTree(const QJsonObject &obj)
{
    BovineTree::readInitialTree(obj);
    foreach(BovineNodeMapping* mapping, prop2pathMap) {
        if (mapping->getPropName() == "uid") {
            QVariant* val = mapping->getValue();
            if (val) uidMap[val->toString()] = mapping;
        }
    }
}
