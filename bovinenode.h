#ifndef BOVINENODE_H
#define BOVINENODE_H

#include <QVector>
#include <QHash>
#include <QJsonObject>
#include <QVariant>

class BovineTree;

class BovineNode
{
public:
    BovineNode();
    BovineNode(const QJsonObject &json, BovineTree *map,
               BovineNode *parent = nullptr);
    ~BovineNode();
    BovineNode *findNodeByPath(const QString &path);
    BovineNode *findNodeByPropValue(const QString &propval);
    QString getPath() const;

    QVariant *getProperty(const QString &key);
    QVector<BovineNode*>* getChildren() const;

    QString getFullPath() const;
    void setFullPath(const QString &value);

    QHash<QString, QVariant>* getProperties() const;

    void setProperty(QString name, QVariant val);

private:
    BovineTree *pathMap;
    BovineNode *parent;
    QVector<BovineNode*>* children;
    QString path;
    QString fullPath;
    QHash<QString, QVariant>* properties;
    void parseJSON(const QJsonObject &json);
};

#endif // BOVINENODE_H
