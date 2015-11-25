#ifndef XMLTREEMODEL_H
#define XMLTREEMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QDomNode>

class QDomDocument;

namespace XmlTreeEditor {
namespace Internal {

class XmlTreeNode;
class XmlTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum XmlTreeModelRoles {
        NodeTypeRole = Qt::UserRole,
        IsLeafRole
    };

    explicit XmlTreeModel(QObject *parent = 0);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex	parent(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool addNode(QDomNode::NodeType nodeType = QDomNode::ElementNode, const QString &nodeName = QString(), const QString &nodeValue = QString(), const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    QStringList mimeTypes() const;
    Qt::DropActions supportedDragActions() const;
    Qt::DropActions supportedDropActions() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

    bool populateFromXml(const QString &xmlContent);
    QString getXml();

signals:
    void xmlModified();

public slots:

private:
    QByteArray serializeNodesForDragAndDrop(const QList<XmlTreeNode *> &nodes) const;
    QList<XmlTreeNode *> deserializeNodesForDragAndDrop(const QByteArray &serialized);
    int nodeRow(XmlTreeNode *node) const;
    bool parseNode(const QDomNode &node, XmlTreeNode *parent);
    bool createNode(XmlTreeNode *item, QDomNode *parentNode, QDomDocument *document) const;
    QList<XmlTreeNode *> nodes;
};

}
}
#endif // XMLTREEMODEL_H
