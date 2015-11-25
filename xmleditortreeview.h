#ifndef XMLEDITORTREEVIEW_H
#define XMLEDITORTREEVIEW_H

#include <QTreeView>
#include <texteditor/fontsettings.h>

namespace XmlTreeEditor {

class XmlEditorTreeViewPrivate;
class XmlEditorTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit XmlEditorTreeView(QWidget *parent = 0);
    ~XmlEditorTreeView();
    void setContent(const QString &content);
    QString content();

    void setFontSettings(const TextEditor::FontSettings &fs);

signals:
    void contentModified();

protected slots:
    void onContentModified();

protected:
    XmlEditorTreeViewPrivate *d;
    void contextMenuEvent(QContextMenuEvent *ev);
    void dropEvent(QDropEvent *event);

private:
    QMenu *contextMenu;
    QAction *addSubElementAction;
    QAction *addAttributeAction;
    QAction *createRootElementAction;
    QAction *deleteNodeAction;
    friend class XmlEditorTreeViewPrivate;

};

}
#endif // XMLEDITORTREEVIEW_H
