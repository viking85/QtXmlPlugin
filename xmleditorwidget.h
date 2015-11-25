#ifndef XMLEDITORWIDGET_H
#define XMLEDITORWIDGET_H

#include <QTabWidget>

namespace Core
{
    class IFile;
}


namespace XmlTreeEditor {

class XmlEditorTreeView;
class XmlSourceEditor;

class XmlEditorWidgetPrivate;
class XmlEditorWidget : public QTabWidget
{
    Q_OBJECT
public:
    enum EditorState
    {
        TreeViewEditor,
        SourceEditor
    };

    explicit XmlEditorWidget(QWidget *parent = 0);
    ~XmlEditorWidget();

    Core::IFile *file();
    bool createNew(const QString &contents);
    bool open(const QString &fileName = QString());
    QByteArray saveState() const;
    bool restoreState(const QByteArray &state);
    QString displayName() const;
    void setDisplayName(const QString &title);
    EditorState currentEditorState();
    void updateTab(EditorState tab);

    XmlEditorTreeView *treeView();
    XmlSourceEditor *sourceEditor();

signals:

public slots:
    void documentSaved();

protected:
    XmlEditorWidgetPrivate *d;

private slots:
    void slotCurrentChanged(int index);
    void slotTreeContentModified();
    void slotSourceContentModified();

private:
    friend class XmlEditorWidgetPrivate;

    void setToolbarLineColumnLabelVisible(bool visible);

};

}
#endif // XMLEDITORWIDGET_H
