#include "xmltreevaluedelegate.h"
#include <QDomNode>
#include <QTextDocument>
#include <QTextCursor>
#include <QPainter>
#include "xmltreemodel.h"

using namespace XmlTreeEditor::Internal;

QTextCharFormat XmlTreeValueDelegate::valueFormat = QTextCharFormat();
QTextCharFormat XmlTreeValueDelegate::selectedFormat = QTextCharFormat();
QTextCharFormat XmlTreeValueDelegate::commentFormat = QTextCharFormat();

XmlTreeValueDelegate::XmlTreeValueDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void XmlTreeValueDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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
        case QDomNode::CommentNode:
            cursor.mergeCharFormat(commentFormat);
            break;
        default:
            cursor.mergeCharFormat(valueFormat);
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

QSize XmlTreeValueDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
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
