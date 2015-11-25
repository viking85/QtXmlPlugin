#include "xmltreemodel.h"
#include "xmleditorconstants.h"

#include <QtXml>
#include <QList>

#include <QDebug>

using namespace XmlTreeEditor;
using namespace XmlTreeEditor::Internal;


namespace XmlTreeEditor {
namespace Internal {
class XmlTreeNode
{
public:
    XmlTreeNode(): nodeType(QDomNode::BaseNode), parentNode(0), isLeaf(true) {}

    ~XmlTreeNode()
    {
        qDeleteAll(childNodes);
    }

    QString nodeName;
    QString nodeValue;
    QDomNode::NodeType nodeType;

    QString nodeTypeName()
    {
        switch (nodeType)
        {
        case QDomNode::ElementNode:
            return QObject::tr("Element");
        case QDomNode::AttributeNode:
            return QObject::tr("Attribute");
        case QDomNode::TextNode:
            return QObject::tr("Text");
        case QDomNode::CDATASectionNode:
            return QObject::tr("CDATA");
        case QDomNode::CommentNode:
            return QObject::tr("Comment");
        case QDomNode::BaseNode:
        default:
            return QObject::tr("Unknown");
        }
    }

    QList<XmlTreeNode *> childNodes;
    XmlTreeNode *parentNode;
    bool isLeaf;
};

}
}



XmlTreeModel::XmlTreeModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    connect(this, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(xmlModified()));
    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SIGNAL(xmlModified()));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SIGNAL(xmlModified()));
}

QModelIndex XmlTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    XmlTreeNode *childNode = 0;

    if (!parent.isValid())
    {
        if (nodes.size() > row && row >= 0)
            childNode = nodes.at(row);
    }
    else
    {
        XmlTreeNode *parentNode = static_cast<XmlTreeNode *>(parent.internalPointer());
        if (parentNode->childNodes.size() > row && row >= 0)
            childNode = parentNode->childNodes.at(row);
    }

    if (childNode)
        return createIndex(row, column, childNode);
    else
        return QModelIndex();
}

QModelIndex XmlTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    XmlTreeNode *childNode = static_cast<XmlTreeNode *>(index.internalPointer());
    XmlTreeNode *parentNode = childNode->parentNode;
    if (!parentNode)
        return QModelIndex();

    QModelIndex newIndex = createIndex(nodeRow(parentNode), 0, parentNode);

    return newIndex;
}

QVariant XmlTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (role != Qt::DisplayRole)
            return QVariant();
        switch (section)
        {
        case 0: return tr("Name");
        case 1: return tr("Value");
        case 2: return tr("Type");
        default: return QVariant();
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

int XmlTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
    {
        return nodes.size();
    }
    else
    {
        XmlTreeNode *parentNode = static_cast<XmlTreeNode *>(parent.internalPointer());
        return parentNode->childNodes.size();
    }

}

int XmlTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant XmlTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    XmlTreeNode *node;
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        node = static_cast<XmlTreeNode *>(index.internalPointer());
        switch (index.column())
        {
        case 0:
            return node->nodeName;
        case 1:
            return node->nodeValue;
        case 2:
            return node->nodeTypeName();
        default:
            return QVariant();
        }
    case NodeTypeRole:
        node = static_cast<XmlTreeNode *>(index.internalPointer());
        return QVariant(node->nodeType);
    case IsLeafRole:
        node = static_cast<XmlTreeNode *>(index.internalPointer());
        return QVariant(node->isLeaf);
    default:
        return QVariant();
    }
}

bool XmlTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool isModified = false;
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        XmlTreeNode *childNode = static_cast<XmlTreeNode *>(index.internalPointer());
        switch (index.column())
        {
        case 0:
            if (value.toString() != childNode->nodeName)
            {
                isModified = true;
                childNode->nodeName = value.toString();
            }
            break;
        case 1:
            if (value.toString() != childNode->nodeValue)
            {
                isModified = true;
                childNode->nodeValue = value.toString();
            }
            break;
        default:
            return false;
        }
        if (isModified)
            emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags XmlTreeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags;
    flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (index.isValid())
        flags |= Qt::ItemIsDragEnabled;
    if (index.isValid() && index.column() == 0 && index.data(NodeTypeRole).toInt() == QDomNode::ElementNode)
        flags |= Qt::ItemIsDropEnabled;
    if (index.isValid() && (index.column() == 0 || (index.column() == 1 && index.data(IsLeafRole).toBool())))
        flags |= Qt::ItemIsEditable;
    return flags;
}

bool XmlTreeModel::addNode(QDomNode::NodeType nodeType, const QString &nodeName, const QString &nodeValue, const QModelIndex &parent)
{
    XmlTreeNode *toIns = new XmlTreeNode;
    toIns->nodeType = nodeType;
    toIns->nodeName = nodeName;
    toIns->nodeValue = nodeValue;
    if (!parent.isValid())
    {
        if (nodeType == QDomNode::ElementNode)
        {
            int rowStart = nodes.size();
            beginInsertRows(parent, rowStart, rowStart);
            nodes.insert(rowStart, toIns);
            endInsertRows();
        }
        else
        {
            delete toIns;
            return false;
        }
    }
    else
    {
        XmlTreeNode *parentNode = static_cast<XmlTreeNode *>(parent.internalPointer());
        int rowStart = parentNode->childNodes.size();
        if (nodeType == QDomNode::AttributeNode)
        {
            for (rowStart = 0;
                 rowStart < parentNode->childNodes.size() && parentNode->childNodes[rowStart]->nodeType == QDomNode::AttributeNode;
                 ++rowStart) {}
        }
        if (parentNode->isLeaf && nodeType == QDomNode::ElementNode)
        {
            parentNode->isLeaf = false;
            parentNode->nodeValue = "";
        }
        beginInsertRows(parent, rowStart, rowStart);
        toIns->parentNode = parentNode;
        parentNode->childNodes.insert(rowStart, toIns);
        endInsertRows();
    }
    return true;
}

bool XmlTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (!parent.isValid())
    {
        beginRemoveRows(parent, row, row+count);
        while (count && nodes.size()>=row)
        {
            delete nodes[row];
            nodes.removeAt(row);
            --count;
        }
        endRemoveRows();
    }
    else
    {
        XmlTreeNode *parentNode = static_cast<XmlTreeNode *>(parent.internalPointer());
        if (parentNode->nodeType != QDomNode::ElementNode)
            return false;
        beginRemoveRows(parent, row, row+count);
        while (count && parentNode->childNodes.size()>=row)
        {
            delete parentNode->childNodes[row];
            parentNode->childNodes.removeAt(row);
            --count;
        }
        if (parentNode->childNodes.size() == 0)
            parentNode->isLeaf = true;
        else
        {
            int i = 0;
            for (; i < parentNode->childNodes.size() && parentNode->childNodes[i]->nodeType == QDomNode::AttributeNode; ++i) {}
            parentNode->isLeaf = i == parentNode->childNodes.size();
        }
        endRemoveRows();
    }

    return true;
}

QStringList XmlTreeModel::mimeTypes() const
{
    QStringList result;
    result << QLatin1String(Constants::XML_MIME_TYPE);
    return result;
}

Qt::DropActions XmlTreeModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions XmlTreeModel::supportedDragActions() const
{
    return supportedDropActions();
}


QMimeData *XmlTreeModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *resultMimeData = new QMimeData();
    QList<XmlTreeNode *> nodesToDrag;
    foreach (QModelIndex currentIndex, indexes)
    {
         if (!currentIndex.isValid() || currentIndex.column() != 0)
             continue;
         XmlTreeNode *toIns = static_cast<XmlTreeNode *>(currentIndex.internalPointer());
         nodesToDrag << toIns;
    }
    resultMimeData->setData(QLatin1String(Constants::XML_MIME_TYPE), serializeNodesForDragAndDrop(nodesToDrag));
    return resultMimeData;
}

bool XmlTreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(column);

    if (action == Qt::IgnoreAction)
             return true;

    if (!data->hasFormat(QLatin1String(Constants::XML_MIME_TYPE)))
        return false;

    QByteArray serializedNodes = data->data(Constants::XML_MIME_TYPE);

    if (serializedNodes.isEmpty())
        return false;
    QList<XmlTreeNode *> nodesToDrop = deserializeNodesForDragAndDrop(serializedNodes);
    if (nodesToDrop.empty())
        return false;

    XmlTreeNode *parentNode = 0;
    if (!parent.isValid())
    {
        if (nodes.size())
            parentNode = nodes[0];
    }
    else
    {
        parentNode = static_cast<XmlTreeNode *>(parent.internalPointer());
    }

    if (!parentNode)
        return false;

    if (row == -1)
        row = parentNode->childNodes.size();

    QList<XmlTreeNode *> attributesToDrop;
    QList<XmlTreeNode *> otherNodesToDrop;
    bool elementNodesExist = false;
    foreach (XmlTreeNode *currentNode, nodesToDrop)
    {
        if (currentNode->nodeType == QDomNode::AttributeNode)
            attributesToDrop << currentNode;
        else
            otherNodesToDrop << currentNode;
        if (currentNode->nodeType == QDomNode::ElementNode)
            elementNodesExist = true;
    }

    if (!attributesToDrop.empty())
    {
        int attributesEndRow = 0;
        for (;
             attributesEndRow < parentNode->childNodes.size() && parentNode->childNodes[attributesEndRow]->nodeType == QDomNode::AttributeNode;
             ++attributesEndRow) {}
        if (row > attributesEndRow)
            row = attributesEndRow;
        attributesEndRow += attributesToDrop.size();

        beginInsertRows(parent, row, row+attributesToDrop.size()-1);
        foreach (XmlTreeNode *toIns, attributesToDrop)
        {
            toIns->parentNode = parentNode;
            parentNode->childNodes.insert(row, toIns);
            ++row;
        }
        endInsertRows();

        row = attributesEndRow;
    }


    if (parentNode->isLeaf && elementNodesExist)
    {
        parentNode->isLeaf = false;
        parentNode->nodeValue = "";
    }
    beginInsertRows(parent, row, otherNodesToDrop.size()-1);
    foreach (XmlTreeNode *toIns, otherNodesToDrop)
    {
        toIns->parentNode = parentNode;
        parentNode->childNodes.insert(row, toIns);
        ++row;
    }
    endInsertRows();

    return true;
}


bool XmlTreeModel::populateFromXml(const QString &xmlContent)
{
    beginResetModel();
    qDeleteAll(nodes);
    nodes.clear();
    QDomDocument document;
    if (!document.setContent(xmlContent))
    {
        emit layoutChanged();
        return false;
    }

    QDomNode rootElement = document.documentElement();
    bool result = parseNode(rootElement, 0);
    endResetModel();
    return result;
}


QString XmlTreeModel::getXml()
{
    QDomDocument resultDocument;
    bool result = true;
    foreach (XmlTreeNode* treeNode, nodes)
    {
        result &= createNode(treeNode, &resultDocument, &resultDocument);
        if (!result)
            break;
    }
    if (!result)
        return QString();
    return resultDocument.toString(4);
}


int XmlTreeModel::nodeRow(XmlTreeNode *node) const
{
    if (!node)
        return -1;
    if (node->parentNode)
        return node->parentNode->childNodes.indexOf(node);
    return nodes.indexOf(node);
}


QByteArray XmlTreeModel::serializeNodesForDragAndDrop(const QList<XmlTreeNode *> &nodes) const
{
    QDomDocument resultDocument;
    bool result = true;
    QDomElement rootElement = resultDocument.createElement("dragndrop_data");
    resultDocument.appendChild(rootElement);
    foreach (XmlTreeNode* treeNode, nodes)
    {
        result &= createNode(treeNode, &rootElement, &resultDocument);
        if (!result)
            break;
    }
    if (!result)
        return QByteArray();
    return resultDocument.toByteArray(0);
}

QList<XmlTreeNode *> XmlTreeModel::deserializeNodesForDragAndDrop(const QByteArray &serialized)
{
    QList<XmlTreeNode *> result;
    QDomDocument document;
    if (!document.setContent(serialized))
        return result;

    QDomElement rootElement = document.documentElement();
    if (rootElement.nodeName() != "dragndrop_data")
        return result;


    XmlTreeNode *dummyNode = new XmlTreeNode();
    bool resultFlag = true;

    QDomNamedNodeMap attributes = rootElement.attributes();
    for (int i = 0; i < attributes.size() && resultFlag; ++i)
        resultFlag &= parseNode(attributes.item(i), dummyNode);

    QDomNodeList children = rootElement.childNodes();
    for (int i = 0; i < children.size() && resultFlag; ++i)
        resultFlag &= parseNode(children.at(i), dummyNode);

    if (resultFlag)
        result.append(dummyNode->childNodes);

    dummyNode->childNodes.clear();
    delete dummyNode;
    return result;
}


bool XmlTreeModel::parseNode(const QDomNode &node, XmlTreeNode *parent)
{
    XmlTreeNode *toIns = new XmlTreeNode;
    toIns->parentNode = parent;
    toIns->nodeName = node.nodeName();
    toIns->nodeType = node.nodeType();
    toIns->nodeValue = node.nodeValue();

    if (parent)
        parent->childNodes << toIns;
    else
        nodes << toIns;

    bool result = true;
    if (node.isElement())
    {
        QDomNamedNodeMap attributes = node.attributes();
        for (int i = 0; i < attributes.size(); ++i)
            result &= parseNode(attributes.item(i), toIns);

        QDomNodeList children = node.childNodes();
        if (children.size() == 1 &&
                (children.at(0).isText() || children.at(0).isCDATASection() || children.at(0).isCharacterData()))
        {
            toIns->nodeValue = node.toElement().text();
        }
        else
        {
            toIns->isLeaf = false;
            for (int i = 0; i < children.size() && result; ++i)
                result &= parseNode(children.at(i), toIns);
        }
    }


    return result;
}


bool XmlTreeModel::createNode(XmlTreeNode *item, QDomNode *parentNode, QDomDocument *document) const
{
    bool result = true;
    QDomNode node;

    switch(item->nodeType)
    {
    case QDomNode::ElementNode:
        node = document->createElement(item->nodeName);
        if (!item->nodeValue.isEmpty())
            node.appendChild(document->createTextNode(item->nodeValue));
        break;
    case QDomNode::AttributeNode:
        if (parentNode->isElement())
            parentNode->toElement().setAttribute(item->nodeName, item->nodeValue);
        else
            return false;
        return true;
    case QDomNode::TextNode:
        node = document->createTextNode(item->nodeValue);
        break;
    case QDomNode::CDATASectionNode:
        node = document->createCDATASection(item->nodeValue);
        break;
    case QDomNode::CommentNode:
        node = document->createComment(item->nodeValue);
        break;
    case QDomNode::BaseNode:
    default:
        return false;
    }

    parentNode->appendChild(node);

    foreach (XmlTreeNode *treeNode, item->childNodes)
    {
        result &= createNode(treeNode, &node, document);
        if (!result)
            break;
    }
    return result;
}

#include "xmltreemodel.moc"
