#include "xmltreenamedelegate.h"
#include <QDomNode>
#include <QTextDocument>
#include <QTextCursor>
#include <QPainter>
#include "xmltreemodel.h"

using namespace XmlTreeEditor::Internal;

QTextCharFormat XmlTreeNameDelegate::attributeFormat = QTextCharFormat();
QTextCharFormat XmlTreeNameDelegate::elementFormat = QTextCharFormat();
QTextCharFormat XmlTreeNameDelegate::nodeTypeFormat = QTextCharFormat();
QTextCharFormat XmlTreeNameDelegate::selectedFormat = QTextCharFormat();
QTextCharFormat XmlTreeNameDelegate::genericFormat = QTextCharFormat();


XmlTreeNameDelegate::XmlTreeNameDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void XmlTreeNameDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString str = index.data(Qt::DisplayRole).toString();
    QDomNode::NodeType nodeType = static_cast<QDomNode::NodeType>(index.data(XmlTreeModel::NodeTypeRole).toInt());
    QTextDocument td;

    QStyleOptionViewItemV4 opt = option;

    QRectF optRect = opt.rect;
    QRectF textRect = optRect;
    td.setPageSize(QSize(textRect.width(), textRect.height()));
    td.setHtml(str);
    QTextCursor cursor(&td);
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    if (opt.state & QStyle::State_Selected)
    {
        cursor.mergeCharFormat(selectedFormat);
    }
    else
    {
        switch (nodeType)
        {
        case QDomNode::AttributeNode:
            cursor.mergeCharFormat(attributeFormat);
            break;
        case QDomNode::ElementNode:
            cursor.mergeCharFormat(elementFormat);
            break;
        case QDomNode::CommentNode:
        case QDomNode::TextNode:
        case QDomNode::CDATASectionNode:
            cursor.mergeCharFormat(nodeTypeFormat);
            break;
        default:
            cursor.mergeCharFormat(genericFormat);
            break;
        }
    }

    QRectF lr = QRectF(textRect.x(), textRect.y(), textRect.width(), textRect.height());
    painter->save();
    opt.widget->style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
    painter->translate(lr.topLeft());
    painter->setClipRect(lr.translated(-lr.x(), -lr.y()));
    td.drawContents(painter, QRectF());
    painter->restore();
}

QSize XmlTreeNameDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString str = index.data(Qt::DisplayRole).toString();
    QTextDocument td;
    QStyleOptionViewItemV4 opt = option;
    QRectF optRect = opt.rect;
    QRectF textRect = optRect;
    td.setTextWidth(textRect.width());
    td.setHtml(str);
    QSize result = td.size().toSize();
    return result;
}
