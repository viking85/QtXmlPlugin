#ifndef XMLTREEVALUEDELEGATE_H
#define XMLTREEVALUEDELEGATE_H

#include <QStyledItemDelegate>
#include <QTextCharFormat>

namespace XmlTreeEditor {
namespace Internal {

class XmlTreeValueDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit XmlTreeValueDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    static QTextCharFormat valueFormat;
    static QTextCharFormat selectedFormat;
    static QTextCharFormat commentFormat;

signals:

public slots:

};

}
}

#endif // XMLTREEVALUEDELEGATE_H
