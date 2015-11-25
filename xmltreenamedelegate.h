#ifndef XMLTREENAMEDELEGATE_H
#define XMLTREENAMEDELEGATE_H

#include <QStyledItemDelegate>
#include <QTextCharFormat>


namespace XmlTreeEditor {
namespace Internal {

class XmlTreeNameDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit XmlTreeNameDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    static QTextCharFormat attributeFormat;
    static QTextCharFormat elementFormat;
    static QTextCharFormat nodeTypeFormat;
    static QTextCharFormat selectedFormat;
    static QTextCharFormat genericFormat;
signals:

public slots:

private:

};

}
}

#endif // XMLTREENAMEDELEGATE_H
