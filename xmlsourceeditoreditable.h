#ifndef XMLSOURCEEDITOREDITABLE_H
#define XMLSOURCEEDITOREDITABLE_H
#include <texteditor/plaintexteditor.h>
#include <coreplugin/icontext.h>

namespace XmlTreeEditor {

class XmlSourceEditor;
class XmlEditorWidget;
class XmlSourceEditorEditable : public TextEditor::PlainTextEditorEditable
{
    Q_OBJECT
public:
    XmlSourceEditorEditable(XmlSourceEditor *editor, XmlEditorWidget *widget);
    ~XmlSourceEditorEditable();
    Core::Context context() const;

    bool duplicateSupported() const { return false; }
    Core::IEditor *duplicate(QWidget *parent) {Q_UNUSED(parent); return 0;}
    bool isTemporary() const { return false; }
    QString id() const;
    QWidget *widget();
    bool createNew(const QString &contents);
    bool open(const QString &fileName = QString());
    int currentLine() const;
    int currentColumn() const;

private:
    const Core::Context m_context;
    XmlEditorWidget *editorWidget;
};

}

#endif // XMLSOURCEEDITOREDITABLE_H
