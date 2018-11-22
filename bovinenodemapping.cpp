#include "bovinenodemapping.h"

BovineNodeMapping::BovineNodeMapping() :
    userdata(nullptr)
{

}

BovineNodeMapping::BovineNodeMapping(const QString &path,
                                     const QString &propname,
                                     void *userdata) :
    node(nullptr)
{
    this->path = path;
    this->propName = propname;
    this->userdata = userdata;
}

BovineNodeMapping::BovineNodeMapping(BovineNode *node, QString propName) {
    this->node = node;
    this->path = node->getFullPath();
    this->propName = propName;
}

BovineNode *BovineNodeMapping::getNode() const
{
    return node;
}

void BovineNodeMapping::setNode(BovineNode *value)
{
    node = value;
}

QString BovineNodeMapping::getPath() const
{
    return path;
}

/**
 * @brief BovineMapEntry::getValue
 * @return a pointer to the value of the property
 */
QVariant *BovineNodeMapping::getValue()
{
   if (node == nullptr)
       return nullptr;
   QVariant *val = node->getProperty(propName);
   return val;
}


void BovineNodeMapping::setPath(const QString &value)
{
    path = value;
}

QString BovineNodeMapping::getPropName() const
{
    return propName;
}

void BovineNodeMapping::setPropName(const QString &value)
{
    propName = value;
}

void BovineNodeMapping::updateValue(QVariant val)
{
    // Update value in node tree
    if (node != nullptr) node->setProperty(propName, val);
}

void *BovineNodeMapping::getUserdata() const
{
    return userdata;
}

void BovineNodeMapping::setUserdata(void *value)
{
    userdata = value;
}
