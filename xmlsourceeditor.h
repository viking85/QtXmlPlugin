#ifndef XMLSOURCEEDITOR_H
#define XMLSOURCEEDITOR_H

#include <texteditor/plaintexteditor.h>
#include <texteditor/basetextdocument.h>

namespace XmlTreeEditor {
class XmlEditorWidget;
class XmlSourceEditor : public TextEditor::PlainTextEditor
{
    Q_OBJECT
public:
    explicit XmlSourceEditor(XmlEditorWidget *widget, QWidget *parent = 0);
    ~XmlSourceEditor();
    TextEditor::BaseTextEditorEditable *createEditableInterface();

public slots:
    void setFontSettings(const TextEditor::FontSettings &fs);


private slots:
    void performPreSave();

private:
    XmlEditorWidget *editorWidget;
    friend class XmlEditorWidget;
};

namespace Internal {
class XmlSourceDocument : public TextEditor::BaseTextDocument
{
    Q_OBJECT
public:
    XmlSourceDocument() : TextEditor::BaseTextDocument() {}
    bool save(const QString &fileName = QString());
signals:
    void saveRequested();
    void saveFinished();
};

}

}
#endif // XMLSOURCEEDITOR_H
