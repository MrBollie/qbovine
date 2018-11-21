#include "bovinenode.h"
#include "bovinemap.h"
#include <QJsonArray>
#include <QDebug>

BovineNode::BovineNode()
{

}

BovineNode::BovineNode(const QJsonObject &json, BovineMap *map,
                       BovineNode *parent) {
    this->parent = parent;
    this->pathMap = map;
    parseJSON(json);
}

BovineNode::~BovineNode()
{
    foreach(BovineNode* pn, children) {
        delete pn;
    }
}

BovineNode *BovineNode::findNodeByPropValue(const QString &propval)
{
    foreach(QString key, properties.keys()) {

        if (properties[key].toString() == propval)
            return this;
    }
    foreach(BovineNode* n, children) {
        return n->findNodeByPropValue(propval);
    }
    return nullptr;
}

BovineNode *BovineNode::findNodeByPath(const QString &path) {
    QString p = path;
    if (p.startsWith('/')) {
        p = p.remove(0,1);
    }
    QStringList list = p.split('/');
    if (list.size() > 0) {
        if (list[0] == this->path) {
            list.removeFirst();
            if (list.size() == 0) {
                return this;
            }
            else {
                for (int i = 0 ; i < children.size() ; ++i) {
                    BovineNode *res = children[i]->findNodeByPath(list.join("/"));
                    if (res)
                        return res;
                }
            }
        }
    }
    return nullptr;
}

QString BovineNode::getPath() const
{
    return path;
}

// /schnurz/blah/suelz

void BovineNode::parseJSON(const QJsonObject &json) {

    // Clear first
    children.clear();
    properties.clear();

    if (json.contains("path") && json["path"].isString()) {
        path = json["path"].toString();
        if (path.indexOf('/') == 0)
            path.remove(0,1);

        // Full path
        QString fpath = (parent == nullptr ? "" : parent->getFullPath());
        fpath += "/" + path;
        setFullPath(fpath);

        // the root object is different
        QJsonObject obj = (parent == nullptr && json.contains("data") ?
                               json["data"].toObject() : json);

        if (obj.contains("properties") && obj["properties"].isObject()) {
            QJsonObject props = obj["properties"].toObject();
            foreach(QString key, props.keys()) {
                properties.insert(key, props.value(key).toVariant());
                pathMap->add(this, key);
            }
        }
      /*  else {
            pathMap->add(this);
        }*/

        if (obj.contains("children") && obj["children"].isArray()) {
            foreach(QJsonValue v, obj["children"].toArray()) {
                BovineNode *pn = new BovineNode(v.toObject(), pathMap, this);
                children.append(pn);
            }
        }
    }
}

QVariant *BovineNode::getProperty(const QString &key) {
    if (properties.contains(key)) {
        return &properties[key];
    }
    return nullptr;

}

QVector<BovineNode*> BovineNode::getChildren() const
{
    return children;
}

QString BovineNode::getFullPath() const
{
    return fullPath;
}

void BovineNode::setFullPath(const QString &value)
{
    fullPath = value;
}

QHash<QString, QVariant> BovineNode::getProperties() const
{
    return properties;
}

void BovineNode::setProperty(QString name, QVariant val)
{
    properties[name] = val;
}
